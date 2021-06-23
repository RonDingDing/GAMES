#pragma once
#include "Buffers.hpp"
#include "VectorN.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>

void show_bytes(unsigned char *start, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

namespace Rasterizer
{
    using Byte = unsigned char;
    using Word = unsigned short;
    struct TgaHeader
    {
        Byte id_length;        /* 00h  Size of Image ID field */
        Byte color_map_type;   /* 01h  Color map type */
        Byte image_type;       /* 02h  Image type code */
        Word color_map_start;  /* 03h  Color map origin */
        Word color_map_length; /* 05h  Color map length */
        Byte color_map_depth;  /* 07h  Depth of color map entries */
        Word x_offset;         /* 08h  X origin of image */
        Word y_offset;         /* 0Ah  Y origin of image */
        Word width;            /* 0Ch  Width of image */
        Word height;           /* 0Eh  Height of image */
        Byte pixel_depth;      /* 10h  Image pixel size */
        Byte image_descriptor; /* 11h  Image descriptor byte */
    };

    class TgaImage
    {
    public:
        int width = 0;
        int height = 0;
        int bytespp = 0;
        std::vector<Vector3D> buffer = {};

        enum Format
        {
            GRAYSCALE = 1,
            RGB = 3,
            RGBA = 4
        };

        TgaImage() : width(0), height(0) {}
        TgaImage(const Buffers &buf) : width(buf.width), height(buf.height), buffer(buf.pixel)
        {
        }

        void draw_to(const std::string &filename)
        {
        }

        bool load(const char *filename)
        {
            std::ifstream tga_file(filename, std::ios::in | std::ios::binary);
            if (!tga_file.is_open())
            {
                std::cout << "Cannot open this file: " << filename << std::endl;
                return false;
            }

            int file_size = read_size(tga_file);
            int version = read_version(tga_file);
            TgaHeader header;
            read_header(tga_file, header);

            std::cout << "File size: " << file_size << std::endl;
            std::cout << "TGA version: " << version << std::endl;
            std::cout << "ID LENGTH: " << (int)header.id_length << std::endl;
            std::cout << "CM TYPE: " << (int)header.color_map_type << std::endl;
            std::cout << "IMAGE TYPE: " << (int)header.image_type << std::endl;
            std::cout << "FIRST MAP ENTRY: " << header.color_map_start << std::endl;
            std::cout << "NUM MAP ENTRIES: " << header.color_map_length << std::endl;
            std::cout << "BYTES PER ENTRY: " << (int)header.color_map_depth << std::endl;
            std::cout << "ORIGIN: " << header.x_offset << "," << header.x_offset << std::endl;
            std::cout << "SIZE: " << header.width << "," << header.height << ":" << (int)header.pixel_depth << std::endl;
            std::cout << "DESCRIPTOR: " << (int)header.image_descriptor << std::endl;

            width = header.width;
            height = header.height;
            bytespp = header.pixel_depth >> 3; // 即/8

            int pixel_num = width * height;
            int pixel_depth = header.pixel_depth;
            int data_size = pixel_num * pixel_depth;

            std::vector<bool> result = read_image_type(header.image_type);
            bool has_color_map = result[0], rle_encoded = result[1], supported = result[2];
            if ((int)(pixel_depth) != 24 && (int)(pixel_depth) != 32)
            {
                std::cout << "Temporarily not supported: pixel_depth = " << pixel_depth << std::endl;
                return false;
            }
            if (!supported)
            {
                std::cout << "Cannot parse this file: " << filename << std::endl;
                return false;
            }
            if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA))
            {
                tga_file.close();
                std::cout << "Bad bpp (or width/height) value." << std::endl;
                return false;
            }
            if (has_color_map)
            {
                if (header.color_map_length == 0 || header.color_map_depth == 0)
                {
                    tga_file.close();
                    std::cout << "Bad bpp (or width/height) value." << std::endl;
                }
                char color_map[header.color_map_length];
                read_image_color_map(tga_file, header.color_map_length, header.color_map_depth, color_map);
            }

            buffer.clear();
            buffer.reserve(width * height);
            if (rle_encoded)
            {
                parse_encoded(tga_file, bytespp, pixel_num, pixel_depth, has_color_map);
            }
            else
            {
                parse_unencoded(tga_file, bytespp, pixel_num, pixel_depth, has_color_map, data_size);
            }
            flip_y();

            return true;
        }

        void print_pixel()
        {
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    std::cout << buffer[(height - j - 1) * width + i] << " ";
                }
                std::cout << std::endl;
            }
        }

        void flip_y()
        {
            for (size_t j = 0; j < (size_t)height / 2; j++)
            {
                for (size_t i = 0; i < (size_t)width; i++)
                {
                    size_t before = (height - j - 1) * width + i;
                    size_t after = j * width + i;
                    auto a = buffer[before];
                    buffer[before] = buffer[after];
                    buffer[after] = a;
                }
            }
        }

        void flip_x()
        {
            for (size_t j = 0; j < (size_t)height; j++)
            {
                for (size_t i = 0; i < (size_t)width / 2; i++)
                {
                    size_t before = j * width + i;
                    size_t after = j * width + width - i - 1;
                    auto a = buffer[before];
                    buffer[before] = buffer[after];
                    buffer[after] = a;
                }
            }
        }

    private:
        void put_color_data_to_buffer(unsigned char *pixel_data, const size_t &pixel_depth, const bool &has_color_map)
        {
            if (has_color_map)
            {
            }
            if (pixel_depth == 24 || pixel_depth == 32)
            {
                buffer.emplace_back(pixel_data[2], pixel_data[1], pixel_data[0]);
            }
        }

        bool parse_unencoded(std::ifstream &tga_file, const size_t &bytespp, const size_t &pixel_num, const size_t &pixel_depth, const bool &has_color_map, const size_t &data_size)
        {
            unsigned char pixel_data[bytespp];
            for (size_t i = 0; i < pixel_num; i++)
            {
                tga_file.read((char *)&pixel_data, bytespp);
                put_color_data_to_buffer(pixel_data, pixel_depth, has_color_map);
            }
            return true;
        }

        bool parse_encoded(std::ifstream &tga_file, const size_t &bytespp, const size_t &pixel_num, const size_t &pixel_depth, const bool &has_color_map)
        {

            size_t current_pixel = 0;
            do
            {
                char head = tga_file.get();
                size_t same_pixel_num;

                if (head & 128)
                {
                    same_pixel_num = (head & 127) + 1;
                    unsigned char pixel_data[bytespp];
                    tga_file.read((char *)&pixel_data, bytespp);

                    for (size_t j = 0; j < same_pixel_num; j++)
                    {
                        put_color_data_to_buffer(pixel_data, pixel_depth, has_color_map);
                        current_pixel++;
                        if (current_pixel > pixel_num)
                        {
                            std::cerr << "Too many pixels read\n";
                            return false;
                        }
                    }
                }
                else
                {
                    same_pixel_num = head + 1;
                    unsigned char pixel_data[bytespp];
                    for (size_t j = 0; j < same_pixel_num; j++)
                    {
                        tga_file.read((char *)&pixel_data, bytespp);
                        put_color_data_to_buffer(pixel_data, pixel_depth, has_color_map);
                        current_pixel++;
                        if (current_pixel > pixel_num)
                        {
                            std::cerr << "Too many pixels read\n";
                            return false;
                        }
                    }
                }

            } while (current_pixel < pixel_num);
            return true;
        }

        void read_image_data(std::ifstream &tga_file, const int &file_size, char *data)
        {
            tga_file.read(data, file_size);
        }

        void read_image_color_map(std::ifstream &tga_file, const size_t &color_map_length, const size_t &color_map_depth, char *color_map)
        {
            size_t image_color_map_size = color_map_length * color_map_depth;
            color_map = new char[image_color_map_size];
            tga_file.read(color_map, image_color_map_size);
        }

        std::vector<bool> read_image_type(int image_type)
        {
            bool has_color_map, rle_encoded, supported = true;
            switch (image_type)
            {
            case 0:
                has_color_map = false;
                rle_encoded = false;
                break;
            case 1:
                has_color_map = true;
                rle_encoded = false;
                break;
            case 2:
                has_color_map = false;
                rle_encoded = false;
                break;
            case 3:
                has_color_map = false;
                rle_encoded = false;
                break;
            case 9:
                has_color_map = true;
                rle_encoded = true;
                break;
            case 10:
                has_color_map = false;
                rle_encoded = true;
                break;
            case 11:
                has_color_map = false;
                rle_encoded = true;
                break;
            default:
                supported = false;
                break;
            }
            return {has_color_map, rle_encoded, supported};
        }

        std::string read_id(std::ifstream &tga_file, const int &length)
        {
            char id[] = "";
            tga_file.read(id, length);
            return std::string(id);
        }

        int read_size(std::ifstream &tga_file)
        {
            tga_file.seekg(0, std::ios::end);
            int file_size = (int)tga_file.tellg();

            return file_size;
        }

        int read_version(std::ifstream &tga_file)
        {
            char version_string[17] = "";
            tga_file.seekg(-18, std::ios::end);
            tga_file.read(version_string, 16);
            int version = (std::string(version_string) == "TRUEVISION-XFILE") ? 2 : 1;
            tga_file.seekg(0, std::ios::beg);
            return version;
        }

        void read_header(std::ifstream &tga_file, TgaHeader &header)
        {
            tga_file.read((char *)(&header.id_length), sizeof(header.id_length));
            tga_file.read((char *)(&header.color_map_type), sizeof(header.color_map_type));
            tga_file.read((char *)(&header.image_type), sizeof(header.image_type));
            tga_file.read((char *)(&header.color_map_start), sizeof(header.color_map_start));
            tga_file.read((char *)(&header.color_map_length), sizeof(header.color_map_length));
            tga_file.read((char *)(&header.color_map_depth), sizeof(header.color_map_depth));
            tga_file.read((char *)(&header.x_offset), sizeof(header.x_offset));
            tga_file.read((char *)(&header.y_offset), sizeof(header.y_offset));
            tga_file.read((char *)(&header.width), sizeof(header.width));
            tga_file.read((char *)(&header.height), sizeof(header.height));
            tga_file.read((char *)(&header.pixel_depth), sizeof(header.pixel_depth));
            tga_file.read((char *)(&header.image_descriptor), sizeof(header.image_descriptor));
            return;
        }
    };
}