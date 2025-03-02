#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>

namespace base64safe
{
	constexpr char kEncodeLookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	constexpr char kPadCharacter = '=';

	using byte = std::uint8_t;

	inline std::string encode(const std::string &input)
	{
		std::string encoded;
		encoded.reserve(((input.size() + 2) / 3) * 4);

		auto it = input.begin();

		for(std::size_t i = 0; i < input.size() / 3; ++i)
		{
			std::uint32_t temp = static_cast<byte>(*it++) << 16;
			temp += static_cast<byte>(*it++) << 8;
			temp += static_cast<byte>(*it++);
			encoded.append(1, kEncodeLookup[(temp & 0x00FC0000) >> 18]);
			encoded.append(1, kEncodeLookup[(temp & 0x0003F000) >> 12]);
			encoded.append(1, kEncodeLookup[(temp & 0x00000FC0) >> 6 ]);
			encoded.append(1, kEncodeLookup[(temp & 0x0000003F)      ]);
		}

		switch(input.size() % 3)
		{
			case 1: {
				std::uint32_t temp = static_cast<byte>(*it++) << 16;
				encoded.append(1, kEncodeLookup[(temp & 0x00FC0000) >> 18]);
				encoded.append(1, kEncodeLookup[(temp & 0x0003F000) >> 12]);
				encoded.append(2, kPadCharacter);
			} break;
			case 2: {              
					std::uint32_t temp  = static_cast<byte>(*it++) << 16;
					temp += static_cast<byte>(*it++) << 8;
					encoded.append(1, kEncodeLookup[(temp & 0x00FC0000) >> 18]);
					encoded.append(1, kEncodeLookup[(temp & 0x0003F000) >> 12]);
					encoded.append(1, kEncodeLookup[(temp & 0x00000FC0) >> 6 ]);
					encoded.append(1, kPadCharacter);
			} break;							
		}

		return encoded;
	}

	inline std::string decode(const std::string& input)
	{
		if(input.length() % 4)
			throw std::runtime_error("Invalid base64 length!");

		std::size_t padding{};

		if(input.length())
		{
			if(input[input.length() - 1] == kPadCharacter) padding++;
			if(input[input.length() - 2] == kPadCharacter) padding++;
		}

		std::vector<byte> decoded;
		decoded.reserve(((input.length() / 4) * 3) - padding);

		std::uint32_t temp{};
		auto it = input.begin();

		while(it < input.end())
		{
			for(std::size_t i = 0; i < 4; ++i)
			{
				temp <<= 6;
				if     (*it >= 0x41 && *it <= 0x5A) temp |= *it - 0x41;
				else if(*it >= 0x61 && *it <= 0x7A) temp |= *it - 0x47;
				else if(*it >= 0x30 && *it <= 0x39) temp |= *it + 0x04;
				else if(*it == 0x2D)                temp |= 0x3E;
				else if(*it == 0x5F)                temp |= 0x3F;
				else if(*it == kPadCharacter)
				{
					switch(input.end() - it)
					{
					case 1:
						decoded.push_back((temp >> 16) & 0x000000FF);
						decoded.push_back((temp >> 8 ) & 0x000000FF);
						return std::string(decoded.begin(), decoded.end());
					case 2:
						decoded.push_back((temp >> 10) & 0x000000FF);
						return std::string(decoded.begin(), decoded.end());
					default:
						throw std::runtime_error("Invalid padding in safe base64!");
					}
				}
				else throw std::runtime_error("Invalid character in safe base64!");

				++it;
			}

			decoded.push_back((temp >> 16) & 0x000000FF);
			decoded.push_back((temp >> 8 ) & 0x000000FF);
			decoded.push_back((temp      ) & 0x000000FF);
		}

		return std::string(decoded.begin(), decoded.end());
	}
}
