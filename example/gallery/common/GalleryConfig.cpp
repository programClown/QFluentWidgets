#include "GalleryConfig.h"

GalleryConfig *GalleryConfig::cfg()
{
    static GalleryConfig *config;
    if (config == nullptr) {
        config = new GalleryConfig();
    }
    return config;
}
