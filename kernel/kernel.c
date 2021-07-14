void _start() {
    // Create a pointer to a char, point it to the first
    // text cell of video memory (the top left of the screen)
    char* video_memory = (char*) 0xb8000;
    // At the address being pointed to by video_memory
    // store the char 'X' (display 'X' at the top left of the screen)
    *video_memory = 'X';
}