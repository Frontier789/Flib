#include <FRONTIER/System/Matrix.hpp>
#include <FRONTIER/Gui/PlotTools.hpp>
#include <FRONTIER/System/Quaternion.hpp>
#include <FRONTIER/Gui/ClickListener.hpp>
#include <FRONTIER/Gui/ScrollListener.hpp>
#include <FRONTIER/Graphics/DrawData.hpp>
#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/OpenGL.hpp>

namespace fgui
{
	namespace {
		using namespace fg;
		using namespace fm;
		using namespace fw;
		
		class MeshPlotter : public GuiElement, public ClickListener, public ScrollListener
		{
			DrawData m_dd;
			DrawData m_ntbDD;
			ShaderManager m_shader;
			mat4 m_rot;
			float m_camStartZ;
			Camera &m_cam;
			fm::box3f m_aabb;
			PlotOptions m_opts;
			bool m_wireFrame;
			bool m_drawNorms;
			bool m_drawTans;
			bool m_cullCW;
			
			void makeNTB(Mesh &m)
			{
				m_ntbDD.reset();
				
				Mesh ntb_mesh;
				ntb_mesh.pts.reserve(m.norms.size()*2 + m.tans.size()*2 + m.bitans.size()*2);
				ntb_mesh.clr.reserve(ntb_mesh.pts.size());
				
				double scl = 1;
				
				for (auto desc : {make_pair(&m.norms,  vec4(1,0,0)),
								  make_pair(&m.tans,   vec4(0,1,0)),
								  make_pair(&m.bitans, vec4(0,0,1))
								  })
				C(desc.first->size()) {
					ntb_mesh.pts.push_back(m.pts[i]);
					ntb_mesh.pts.push_back(m.pts[i] + desc.first->at(i) * scl);
					ntb_mesh.clr.push_back(desc.second);
					ntb_mesh.clr.push_back(desc.second);
				}
				
				ntb_mesh.faces.push_back(Mesh::Face(fg::Lines,0,m.norms.size()*2));
				ntb_mesh.faces.push_back(Mesh::Face(fg::Lines,m.norms.size()*2,m.tans.size()*2 + m.bitans.size()*2));
				
				m_ntbDD = ntb_mesh;
			}

			void setMesh(Mesh &m)
			{
				makeNTB(m);
				
				m_dd = m;
				m_aabb = m.AABB();
				
				m_camStartZ = m_aabb.size.d + m_aabb.pos.z + vec2(m_aabb.size).max();
				
				m_cam.setPosition(vec3(0,0,m_camStartZ)).lookAt(vec3());
			}

			void prepShader()
			{
				auto ptr = ShaderManager::getDefaultShader();
				m_shader = std::move(*ptr);
				delete ptr;
				
				m_cam.set3D(getSize(),vec3());
			}
			
			void camp(vec3 p)
			{
				p.z = std::max(p.z,m_aabb.pos.z + m_aabb.size.length());
				
				m_cam.setPosition(p);
			}
			
		public:
			void onScroll(float delta) override
			{
				camp(m_cam.getPosition() * std::pow(.7,delta));
			}

			MeshPlotter(GuiContext &cont,vec2 size,Mesh m,PlotOptions opts) : GuiElement(cont, size),
				m_cam(m_shader.getCamera()),
				m_opts(opts),
				m_wireFrame(false),
				m_drawNorms(false),
				m_drawTans(false),
				m_cullCW(false)
			{
				prepShader();
				setMesh(m);
			}
				
			void onDraw(ShaderManager &) override
			{
				if (m_cullCW) {
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
				} else {
					glDisable(GL_CULL_FACE);
				}
				
				if (m_wireFrame || m_drawNorms || m_drawTans) {
					glEnable(GL_POLYGON_OFFSET_FILL);
					glPolygonOffset(1, 1);
				}
				
				m_shader.getModelStack().top(m_rot);
				m_shader.draw(m_dd);
				
				glDisable(GL_POLYGON_OFFSET_FILL);
				
				if (m_wireFrame) {
					mat4 cm(0);
					cm[3][3] = 1;
					m_shader.getColorStack().push(cm);
					
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					m_shader.draw(m_dd);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					
					m_shader.getColorStack().pop();
				}
				
				if (m_drawNorms)
					m_shader.draw(m_ntbDD,0);
				
				if (m_drawTans)
					m_shader.draw(m_ntbDD,1);
			}
			
			void onMouseMove(fm::vec2 p,fm::vec2 prevP) override
			{
				if (isPressed(Mouse::Left)) {
					vec2 delta = p - prevP;
					
					auto r = m_shader.getCamera().r();
					auto u = m_shader.getCamera().v().cross(r);
					
					m_rot = (Quat(u,delta.x / 50 * m_opts.mouseSensitivity) * 
							 Quat(r,-delta.y / 50 * m_opts.mouseSensitivity)).getMatrix() * m_rot;
				}
				if (isPressed(Mouse::Right)) {
					float d = p.y - prevP.y;
					
					camp(m_cam.getPosition() + fm::vec3(0,0,d * m_camStartZ * 0.006 * m_opts.scrollScale));
				}
			}
			
			bool onEvent(Event &ev) override
			{
				if (ev.type == Event::Resized) {
					setSize(ev.size);
					m_shader.getCamera().setCanvasSize(ev.size);
				}
				
				if (ev.type == Event::KeyPressed) {
					if (ev.key.code == Keyboard::W)
						m_wireFrame = !m_wireFrame;
					
					if (ev.key.code == Keyboard::N)
						m_drawNorms = !m_drawNorms;
					
					if (ev.key.code == Keyboard::T)
						m_drawTans  = !m_drawTans;
					
					if (ev.key.code == Keyboard::C)
						m_cullCW    = !m_cullCW;
				}
				
				return false;
			}
		};
	}
	
	/////////////////////////////////////////////////////////////
	GuiPlot plot(Mesh m,PlotOptions opts)
	{
		Mesh *keeper = new Mesh(std::move(m));
		
		return GuiPlot(vec2(640,480),"plot",[keeper,opts](GuiWindow &win) {
			win.setClearColor(opts.bgColor);
			
			auto plotter = new MeshPlotter(win, win.getSize(), std::move(*keeper), opts);
			delete keeper;
			return plotter;
		});
	}
}
