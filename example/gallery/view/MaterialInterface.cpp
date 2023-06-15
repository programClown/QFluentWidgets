#include "MaterialInterface.h"

#include "common/Translator.h"
#include "common/GalleryConfig.h"

#include <Widgets/AcrylicLabel.h>

MaterialInterface::MaterialInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->material, "qfluentwidgets.components.widgets", parent)
{
    return;
    GalleryConfig *cfg = GalleryConfig::cfg();
    m_label->setImage(":/resource/images/chidanta.jpg");
    m_label->setMaximumSize(787, 579);
    m_label->setMinimumSize(197, 145);

    addExampleCard(tr("Acrylic label"), m_label,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/acrylic_label/demo.py", 1);
}

void MaterialInterface::onBlurRadiusChanged(int radius)
{
    m_label->setBlurRadius(radius);
    m_label->setImage(":/resource/images/chidanta.jpg");
}
