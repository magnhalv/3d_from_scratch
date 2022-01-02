int light_apply_intensity(int original_color, float percentage) {
    int a = (original_color & 0xFF000000);
    int r = (original_color & 0x00FF0000) * percentage;
    int g = (original_color & 0x0000FF00) * percentage;
    int b = (original_color & 0x000000FF) * percentage;

    int result = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
    return result;
}