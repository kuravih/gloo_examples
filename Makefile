all: 00_simple/00_simple 01_simple_texture/01_simple_texture 02_simple_mesh/02_simple_mesh 03_render_texture/03_render_texture 04_simple_texture_imgui/04_simple_texture_imgui 05_simple_mesh_imgui/05_simple_mesh_imgui 06_infinite_plane_camera/06_infinite_plane_camera 07_infinite_plane_camera_imgui/07_infinite_plane_camera_imgui 08_multiple_textures/08_multiple_textures 09_multiple_textures_imgui/09_multiple_textures_imgui 10_2dsdf_testing_imgui/10_2dsdf_testing_imgui 11_gpu_cpu_compare/11_gpu_cpu_compare 12_multiple_shaders_imgui/12_multiple_shaders_imgui 13_multiple_windows/13_multiple_windows 14_opencv_imgui/14_opencv_imgui

00_simple/00_simple:
	make -C "./00_simple"

01_simple_texture/01_simple_texture:
	make -C "./01_simple_texture"

02_simple_mesh/02_simple_mesh:
	make -C "./02_simple_mesh"

03_render_texture/03_render_texture:
	make -C "./03_render_texture"

04_simple_texture_imgui/04_simple_texture_imgui:
	make -C "./04_simple_texture_imgui"

05_simple_mesh_imgui/05_simple_mesh_imgui:
	make -C "./05_simple_mesh_imgui"

06_infinite_plane_camera/06_infinite_plane_camera:
	make -C "./06_infinite_plane_camera"

07_infinite_plane_camera_imgui/07_infinite_plane_camera_imgui:
	make -C "./07_infinite_plane_camera_imgui"

08_multiple_textures/08_multiple_textures:
	make -C "./08_multiple_textures"

09_multiple_textures_imgui/09_multiple_textures_imgui:
	make -C "./09_multiple_textures_imgui"

10_2dsdf_testing_imgui/10_2dsdf_testing_imgui:
	make -C "./10_2dsdf_testing_imgui"

11_gpu_cpu_compare/11_gpu_cpu_compare:
	make -C "./11_gpu_cpu_compare"

12_multiple_shaders_imgui/12_multiple_shaders_imgui:
	make -C "./12_multiple_shaders_imgui"

13_multiple_windows/13_multiple_windows:
	make -C "./13_multiple_windows"

14_opencv_imgui/14_opencv_imgui:
	make -C "./14_opencv_imgui"

clean:
	make clean -C "./00_simple"
	make clean -C "./01_simple_texture"
	make clean -C "./02_simple_mesh"
	make clean -C "./03_render_texture"
	make clean -C "./04_simple_texture_imgui"
	make clean -C "./05_simple_mesh_imgui"
	make clean -C "./06_infinite_plane_camera"
	make clean -C "./07_infinite_plane_camera_imgui"
	make clean -C "./08_multiple_textures"
	make clean -C "./09_multiple_textures_imgui"
	make clean -C "./10_2dsdf_testing_imgui"
	make clean -C "./11_gpu_cpu_compare"
	make clean -C "./12_multiple_shaders_imgui"
	make clean -C "./13_multiple_windows"
	make clean -C "./14_opencv_imgui"