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

        void load(const char *filename)
        {
            std::ifstream tga_file(filename, std::ios::in | std::ios::binary);
            if (!tga_file.is_open())
            {
                std::cout << "Cannot open this file: " << filename << std::endl;
                return;
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
            int data_size = pixel_num * bytespp;

            char *data = new char[data_size];

            std::string id = read_id(tga_file, header.id_length);
            std::cout << id << std::endl;

            std::vector<bool> result = read_image_type(header.image_type);
            bool has_color_map = result[0], rle_encoded = result[1], supported = result[2];
            if (bytespp != 3 && bytespp != 4)
            {
                std::cout << "Temporarily not supported: bytespp = " << bytespp << std::endl;
                return;
            }
            if (!supported)
            {
                std::cout << "Cannot parse this file: " << filename << std::endl;
                return;
            }
            if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA))
            {
                tga_file.close();
                std::cout << "Bad bpp (or width/height) value." << std::endl;
                return;
            }
            char *color_map;
            if (has_color_map)
            {
                if (header.color_map_length == 0 || header.color_map_depth == 0)
                {
                    tga_file.close();
                    std::cout << "Bad bpp (or width/height) value." << std::endl;
                }
                char *color_map = new char[header.color_map_length];
                read_image_color_map(tga_file, header.color_map_length, header.color_map_depth, color_map);
            }

            read_image_data(tga_file, data_size, data);
            if (rle_encoded)
            {
                parse_encoded(bytespp, data_size, data);
            }

            if (has_color_map)
            {
                convert_data_unmapped(data, color_map);
            }
            else
            {
                convert_data_mapped(data, bytespp, pixel_num);
            }

            delete[] data;
            delete[] color_map;
        }

    private:
        void convert_data_mapped(char *data, const int &bytespp, const int &pixel_num)
        {
            buffer.clear();

            for (int i = 0; i < pixel_num; i += bytespp)
            {
                if (bytespp == 4 || bytespp == 3)
                {
                    buffer.push_back({data[i + 2], data[i + 1], data[i]});
                    // bgr
                }
            }
        }

        void convert_data_unmapped(char *data, char *color_map)
        {
        }

        void parse_encoded(const int &bytespp, const int &data_size, char *data)
        {
            char *temp_data = new char[data_size];
            memcpy(temp_data, data, data_size);

            int current_byte = 0;

            int bytes_per_rle = bytespp + 1;

            for (int i = 0; current_byte < data_size;)
            {
                int run_count = (127 & temp_data[i]) + 1;

                if (128 & temp_data[i])
                {
                    for (int j = 0; j < run_count; j++)
                    {
                        for (int k = 1; k < bytes_per_rle; k++)
                            data[current_byte++] = temp_data[i + k];
                    }

                    i += bytes_per_rle;
                }

                else
                {
                    i++;
                    for (int j = 0; j < run_count; j++)
                    {
                        for (int k = 0; k < bytespp; k++)
                            data[current_byte++] = temp_data[i + k];

                        i += bytespp;
                    }
                }
            }

            delete[] temp_data;
        }

        void read_image_data(std::ifstream &tga_file, const int &data_size, char *data)
        {
            tga_file.read(data, data_size);
        }

        void read_image_color_map(std::ifstream &tga_file, const int &color_map_length, const int &color_map_depth, char *color_map)
        {
            if (color_map)
            {
                delete[] color_map;
            }
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