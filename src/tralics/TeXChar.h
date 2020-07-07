#pragma once

struct TeXChar {
    short int width_idx;  // index in the width_table of this char
    short int height_idx; // index in the height_table of this char
    short int depth_idx;  // index in the depth_table of this char
    short int italic_idx; // index in the italics_table of this char
    short int tag;        // explains how to interpret remainder
    short int remainder;  // the remainder field
};
