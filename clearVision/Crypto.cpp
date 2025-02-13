#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image=secret_image.reconstruct();    
    // 2. Calculate the image dimensions.
    int total_pixels=image.get_height()*image.get_width();
    // 3. Determine the total bits required based on message length.
    int total_bits=7*message_length;
    // 4. Ensure the image has enough pixels; if not, throw an error.
    if (total_bits>total_pixels)
    {
        std::cerr << "Error"<< std::endl;
        exit(1);   
    }
    
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    int start_index = total_pixels - total_bits;
    // 6. Extract LSBs from the image pixels and return the result.
    for (int i = 0; i < total_bits; i++) 
    {
        int pixel_index = start_index + i;
        int row = pixel_index / image.get_height();
        int col = pixel_index % image.get_width();

        int current_pixel = image.get_pixel(row, col);
        std::bitset<8> bits(current_pixel); 
        LSB_array.push_back(bits[0]); 
    }

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size()%7!=0)
    {
        std::cerr << "Error: the LSB array size is not a multiple of 7"<< std::endl;
        exit(1);
    }
    for (int i = 0; i < LSB_array.size(); i += 7) {
        std::bitset<7> binary;

        for (int j = 0; j < 7; j++) 
        {
            binary[6 - j] = LSB_array[i + j]; 
        }

        char character = static_cast<char>(binary.to_ulong());
        message += character;
    }
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.
    for (int i = 0; i < message.length(); i++)
    {
        std::bitset<7> binary(message[i]);
        for (int j = 6; j >= 0; j--) {
            LSB_array.push_back(binary[j]); 
        }
    }
    
    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    SecretImage secret_image(image);
    // TODO: Your code goes here.
    if (image.get_height()*image.get_width()<LSB_array.size())
    {
        std::cerr << "Error: the image has not enough pixels to store the LSB array"<< std::endl;
        exit(1);
    }

    int total_pixels = image.get_height() * image.get_width();
    int start_index = total_pixels - LSB_array.size();


    for (int i = 0; i < LSB_array.size(); i++) 
    {
        int pixel_index = start_index + i;
        int row = pixel_index / image.get_height();
        int col = pixel_index % image.get_width();

        int current_pixel = image.get_pixel(row, col);
        std::bitset<8> bits(current_pixel); 
        bits[0] = LSB_array[i]; 
        int new_pixel = static_cast<int>(bits.to_ulong());

        image.set_pixel(row, col, new_pixel);
    }
    
    secret_image.save_back(image);


    
    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.

    return secret_image;
}
