#include "tucanow_gui.hpp"
#include "tucano_widget_impl.hpp"

void Gui::initialize(int width, int height, std::string assets_dir)
{
    gui.setViewportSize (width, height);

    menu_button.setPosition( 10, 10 );
    menu_button.onClick ( [&](){groupbox.toggleDisplay();} );
    menu_button.setTexture ( assets_dir + "menu_button.pam" );
    //menu_button.setHoverTexture ( assets_dir + "reload_button.pam" );
    menu_button.setDimensionsFromHeight(30);
    gui.add(&menu_button);

    int yoffset = 50;
    groupbox.setPosition (1, 1 + yoffset);
    groupbox.setDimensions (100, 220);
    groupbox.setTexture (assets_dir + "groupbox.pam");
    gui.add(&groupbox);

    reload_button.setPosition( 10, 10 + yoffset );
    reload_button.onClick ( [&](){scene.pimpl->phong.reloadShaders();} );
    reload_button.setTexture ( assets_dir + "reload_button.pam" );
    reload_button.setDimensionsFromHeight(30);
    groupbox.add(&reload_button);

    diffuse_label.setPosition(10, 50 + yoffset);
    diffuse_label.setTexture(assets_dir + "label_diffuse.pam");
    diffuse_label.setDimensionsFromHeight(12);
    groupbox.add(&diffuse_label);

    kd_slider.setPosition(10, 70 + yoffset);
    kd_slider.setDimensions(80, 10);
    kd_slider.onValueChanged( [&](float v){scene.pimpl->phong.setDiffuseCoeff(v); std::cout << "DiffuseCoeff: " << v <<"\n";} );
    kd_slider.setTexture(assets_dir + "slider_bar.pam", assets_dir + "slider.pam");
    kd_slider.moveSlider(scene.pimpl->phong.getDiffuseCoeff());
    groupbox.add(&kd_slider);

    specular_label.setPosition(10, 90 + yoffset);
    specular_label.setTexture(assets_dir + "label_specular.pam");
    specular_label.setDimensionsFromHeight(12);
    groupbox.add(&specular_label);

    ks_slider.setPosition(10, 110 + yoffset);
    ks_slider.setDimensions(80, 10);
    ks_slider.onValueChanged( [&](float v){scene.pimpl->phong.setSpecularCoeff(v); std::cout << "SpecularCoeff: " << v <<"\n";} );
    ks_slider.setTexture(assets_dir + "slider_bar.pam", assets_dir + "slider.pam");
    ks_slider.moveSlider(scene.pimpl->phong.getSpecularCoeff());
    groupbox.add(&ks_slider);

    shininess_label.setPosition(10, 130 + yoffset);
    shininess_label.setTexture(assets_dir + "label_shininess.pam");
    shininess_label.setDimensionsFromHeight(12);
    groupbox.add(&shininess_label);

    shininess_slider.setPosition(10, 150 + yoffset);
    shininess_slider.setDimensions(80, 10);
    shininess_slider.onValueChanged( [&](float v){scene.pimpl->phong.setShininessCoeff(v); std::cout << "ShininessCoeff: " << v <<"\n";} );
    shininess_slider.setTexture(assets_dir + "slider_bar.pam", assets_dir + "slider.pam");
    shininess_slider.setMinMaxValues(1.0, 100.0);
    shininess_slider.moveSlider(scene.pimpl->phong.getShininessCoeff());
    groupbox.add(&shininess_slider);

    ambient_label.setPosition(10, 170 + yoffset);
    ambient_label.setTexture(assets_dir + "label_ambient.pam");
    ambient_label.setDimensionsFromHeight(12);
    groupbox.add(&ambient_label);

    ka_slider.setPosition(10, 190 + yoffset);
    ka_slider.setDimensions(80, 10);
    ka_slider.onValueChanged( [&](float v){scene.pimpl->phong.setAmbientCoeff(v); std::cout << "AmbientCoeff: " << v <<"\n";} );
    ka_slider.setTexture(assets_dir + "slider_bar.pam", assets_dir + "slider.pam");
    ka_slider.moveSlider(scene.pimpl->phong.getAmbientCoeff());
    groupbox.add(&ka_slider);
}

void Gui::render()
{
    gui.render();
}

bool Gui::leftButtonPressed(float xpos, float ypos)
{
    float scale_width, scale_height;
    scene.getScreenScale(scale_width, scale_height);

    double scaled_xpos = scale_width * xpos;
    double scaled_ypos = scale_height * ypos;

    return gui.leftButtonPressed (scaled_xpos, scaled_ypos);
}

bool Gui::leftButtonReleased(float xpos, float ypos)
{
    float scale_width, scale_height;
    scene.getScreenScale(scale_width, scale_height);

    double scaled_xpos = scale_width * xpos;
    double scaled_ypos = scale_height * ypos;

    return gui.leftButtonReleased (scaled_xpos, scaled_ypos);
}

bool Gui::cursorMove(float xpos, float ypos)
{
    float scale_width, scale_height;
    scene.getScreenScale(scale_width, scale_height);

    double scaled_xpos = scale_width * xpos;
    double scaled_ypos = scale_height * ypos;

    return gui.cursorMove (scaled_xpos, scaled_ypos);
}
