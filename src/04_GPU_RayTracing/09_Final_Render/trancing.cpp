#include "trancing.h"
#include "Defines.h"
#include <vector>

#pragma pack(4)
using namespace Define;

struct Sphere
{
    RayVec3 center = RayVec3(0.0f);
    float radius = 1.0f;
    RayVec3 empty = RayVec3(0.0f);
    int materialIndex = 1;
};

struct Material
{
    RayVec3 albedo = RayVec3(0.0f);
    int materialType = 0;
    RayVec2 empty = RayVec2(0.0f);
    float refractScale = 0.0f;
    float fuzz = 0.0f;
};

struct Camera
{
	RayVec3 w;
	RayVec3 u;
	RayVec3 v;

	RayVec3 origin;
	RayVec3 horizontal;
	RayVec3 vertical;
	RayVec3 lower_left_corner;
	float lens_radius;
};

std::vector<Sphere> world;
void initWorld(std::vector<Sphere>& world)
{
    world.resize(6);

    world[0].center = RayVec3(6.0f, 1.0f, 0.0f);
    world[0].materialIndex = 1;

    world[1].center = RayVec3(2.0f, 1.0f, 0.0f);
    world[1].materialIndex = 2;

    world[2].radius = -0.95f;
    world[2].center = RayVec3(2.0f, 1.0f, 0.0f);
    world[2].materialIndex = 3;

    world[3].center = RayVec3(-2.0f, 1.0f, 0.0f);
    world[3].materialIndex = 4;

    world[4].center = RayVec3(-6.0f, 1.0f, 0.0f);
    world[4].materialIndex = 5;

    world[5].radius = 300.0f;
    world[5].center = RayVec3(0.0f, -300.5f, 0.0f);
    world[5].materialIndex = 6;
}

std::vector<Material> material;
void initMaterial(std::vector<Material>& material)
{
    //第一个定义为空白材质进行占位
    material.resize(7);

    material[0].materialType = 0;
    material[0].albedo = RayVec3(1.0f, 0.0f, 1.0f);

    material[1].materialType = 2;
    material[1].albedo = RayVec3(0.7f, 0.6f, 0.5f);
    material[1].fuzz = Define::random(0.0f, 0.15f);

    material[2].materialType = 3;
    material[2].albedo = RayVec3(0.95f);
    material[2].refractScale = 1.42f;
    material[2].fuzz = Define::random(0.0f, 0.1f);

    material[3].materialType = 3;
    material[3].albedo = RayVec3(1.0f);
    material[3].refractScale = 1.42f;

    material[4].materialType = 1;
    material[4].albedo = RayVec3(0.8f, 0.45f, 0.25f);

    material[5].materialType = 3;
    material[5].albedo = RayVec3(0.65f, 0.839f, 0.98f);
    material[5].refractScale = 1.42f;

    material[6].materialType = 1;
    material[6].albedo = RayVec3(0.5f);
}

void addword(std::vector<Sphere>& world, std::vector<Material>& material)
{
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = random();
            RayVec3 center(a + 0.9f * random(), 0.2f, b + 0.9f * random());
            if ((center - RayVec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
            {
                if (choose_mat < 0.55f)
                {
                    Material mat;
                    mat.albedo = randomVec3(0.0f, 1.0f);
                    mat.materialType = 1;
                    material.push_back(mat);

                    Sphere sphere;
                    sphere.center = center;
                    sphere.radius = 0.2f;
                    sphere.materialIndex = material.size();
                    world.push_back(sphere);
                }
                else if (choose_mat < 0.9f)
                {
                    Material mat;
                    mat.albedo = randomVec3(0.4f, 1.0f);
                    mat.fuzz = random(0.0f, 0.2f);
                    mat.materialType = 2;
                    material.push_back(mat);

                    Sphere sphere;
                    sphere.center = center;
                    sphere.radius = 0.2f;
                    sphere.materialIndex = material.size();
                    world.push_back(sphere);
                }
                else 
                {
                    Material mat;
                    mat.albedo = randomVec3(0.4f, 1.0f);
                    mat.fuzz = random(0.0f, 0.1f);
                    mat.materialType = 3;
                    mat.refractScale = random(1.0f, 1.5f);
                    material.push_back(mat);

                    Sphere sphere;
                    sphere.center = center;
                    sphere.radius = 0.2f;
                    sphere.materialIndex = material.size();
                    world.push_back(sphere);
                }
            }
        }
    }
}

void Trancing::initBuffers()
{
    //创建画布
    std::vector<GLfloat> buff(m_width * m_height * 4, 0.0f);
    glGenBuffers(1, &pixelVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pixelVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, buff.size() * sizeof(GLfloat), &buff[0], GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    initWorld(world);
    initMaterial(material);
    addword(world, material);

    glGenBuffers(1, &shapeVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shapeVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, world.size() * sizeof(Sphere), &world[0], GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &materialVBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, materialVBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, material.size() * sizeof(Material), &material[0], GL_DYNAMIC_COPY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Camera cam;
void initCamera(int width, int height)
{
	static bool neadInit = true;
	if (neadInit)
	{
		RayVec3 lookfrom(13.0f, 4.0f, 4.0f);
		RayVec3 lookat(0.0f, 0.0f, 0.0f);
		RayVec3 vup(0.0f, 1.0f, 0.0f);
		const auto aspect_ratio = 1.0f * width / height;
		auto focus_dist = 10.0f;
		auto aperture = 0.1f;
		// Camera
		float theta = 20.0f * 3.141592653589793f / 180.0f;
		float h = tan(theta / 2.0f);
		float viewport_height = 2.0f * h;
		float viewport_width = aspect_ratio * viewport_height;

		cam.w = normalize(lookfrom - lookat);
		cam.u = normalize(cross(vup, cam.w));
		cam.v = normalize(cross(cam.w, cam.u));

		cam.origin = lookfrom;
		cam.horizontal = focus_dist * viewport_width * cam.u;
		cam.vertical = focus_dist * viewport_height * cam.v;
		cam.lower_left_corner = cam.origin - cam.horizontal / 2.0f - cam.vertical / 2.0f - focus_dist * cam.w;
		cam.lens_radius = aperture / 2.0f;

		neadInit = false;
	}
}
void Trancing::renderEx()
{
	initCamera(m_width, m_height);    
    computeProg.use();
    computeProg.setUniform("viewSize", glm::vec2(m_width, m_height));
    computeProg.setUniform("viewSizeInv", 1.0f / glm::vec2(m_width, m_height));

	computeProg.setUniform("origin", cam.origin);
	computeProg.setUniform("horizontal", cam.horizontal);
	computeProg.setUniform("vertical", cam.vertical);
	computeProg.setUniform("lower_left_corner", cam.lower_left_corner);
	computeProg.setUniform("lens_radius", cam.lens_radius);
	computeProg.setUniform("axisU", cam.u);
	computeProg.setUniform("axisV", cam.v);

    computeProg.setUniform("shpereNum", (int)world.size());
    computeProg.setUniform("currentFrame", m_currentFrame);
    computeProg.setUniform("recursionDepth", m_maxDepth);
    glDispatchCompute((m_width + 7) >> 3, (m_height + 7) >> 3, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Trancing::compileAndLinkShaderEx()
{
    try
    {
        std::string path = ROOT_PATH + Define::shaderPrefix;
        computeProg.compileShader(path + "trancing.comp");
        computeProg.link();
    }
    catch (GLSLProgramException & e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main()
{
    SceneRunner runner(Define::windowTitle, std::unique_ptr<Scene>(new Trancing()));
    return runner.run();
}