#include "main_window.hpp"


int main()
{
    auto& main_window = MainWindow::Get();

    main_window.openMeshFile("./samples/models/toy.ply");
    main_window.setAssetsDir("./samples/assets/");

    const int window_width = 800;
    const int window_height = 800;
    std::string window_title = "A Simple Mesh Viewer";

    return main_window.run(window_width, window_height, window_title);
}
