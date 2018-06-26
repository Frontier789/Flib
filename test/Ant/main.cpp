#include <Frontier.hpp>
#include <iostream>

using namespace std;

#include <random>
std::mt19937_64 mt(42);
std::uniform_real_distribution<double> dst(0,1);
double random01()
{
	return dst(mt);
}

class Plot : public GuiElement
{
	DrawData m_dd;
public:
	Plot(vec2 s,GuiContext &cont) : GuiElement(cont,s) {}
	
	void line(const vector<vec2> &pts,vec4 c)
	{
		Mesh m = Mesh::tesLineStrip(&pts[0],pts.size(),1.3);
		m.clr.resize(m.pts.size(),c);
		
		m_dd = m;
	}
	
	void onDraw(fg::ShaderManager &shader) override
	{
		shader.draw(m_dd);
	}
};

float f(float x)
{
	return 1 - (sin(x*18*sin(x*3))+1) * (x - x*x);
}

template<Size I,Size N,Size H,Size O,class T,Size P>
class GenAlg
{
public:
	typedef NeuralNet<I,N,H,O,T> NN;
	
	class Individual {
	public:
		NN nn;
		T fit;
		vec4 c;
		vector<vec2> plot_pts;
	};
	
	array<Individual*,P> population;
	
	GenAlg() {
		for (auto &ind : population) {
			ind = new Individual;
			ind->plot_pts.resize(64);
			ind->c = vec4(random01(),random01(),random01(),1);
		}
		
		population[0]->nn.loadFromFile("best.nn");
		
		fillPlots();
	}
	
	~GenAlg() {
		for (auto &ind : population) {
			delete ind;
		}
	}
	
	T fitness(const NeuralNet<I,N,H,O,T> &nn) const {
		T diff = T();
		
		C(64)
			diff += abs(T(nn.eval(i/64.0)) - f(i/64.0))/64.0;
		
		return -diff;
	}
	
	void fillFitness() {
		for (auto &ind : population) ind->fit = fitness(ind->nn);
	}
	
	void sortByFitness() {
		std::sort(population.begin(),population.end(),[](const Individual *a,const Individual *b){return a->fit > b->fit;});
	}
	
	void fillPlots() {
		for (auto &ind : population) {
			C(64)
				ind->plot_pts[i] = vec2(i/63.0*640,ind->nn(i/63.0)*480);
		}
	}
	
	void mutate(T chance,T mut) {
		for (auto &ind : population) {
			if (random01() < chance)
				ind->nn.mutate(mut);
		}
	}
	
	void cross(Size elite) {
		for (Size i=elite;i<P;++i) {
			int p1 = random01() * elite;
			int p2 = random01() * elite;
			NN::cross(population[i]->nn,population[p1]->nn,population[p2]->nn);
			T r = random01();
			population[i]->c = population[p1]->c * r + population[p2]->c * (1-r) + vec4(random01()-.5,random01()-.5,random01()-.5,0)*2*.1;
		}
	}
	
	void do_iteration()
	{
		fillFitness();
		sortByFitness();
		
		cross(P*.3);
		mutate(.3,.8);
		
		fillPlots();
	}
};

class NNPlot : public GuiElement
{
	vector<Plot*> plots;
	Clock learn_clk;
	GenAlg<1,2,11,1,float,10> gen;
public:
	NNPlot(vec2 s,GuiContext &cont) : GuiElement(cont,s), db(0)
	{
		vector<vec2> ptsFun;
		C(65)
			ptsFun.push_back(vec2(i*10,f(i/64.0)*480.0));
		
		setPlot(0,ptsFun,vec4::White);
		
		plotGen();
	}
	
	~NNPlot()
	{
		gen.population[0]->nn.saveToFile("best.nn");
	}
	
	void plotGen()
	{
		Size i = 4;
		for (const auto &ind : gen.population) {
			setPlot(i,ind->plot_pts,ind->c);
			++i;
		}
	}
	
	Clock clk;
	int db;
	void onUpdate(const fm::Time &) override
	{
		if (clk.getSeconds() > 0.01) {
			
			for (auto ind : gen.population) {
				for (int i=0;i<1000;++i)
				{
					float k = (random01())*1.4-.2;
					ind->nn.train(k,f(k),4/(1+.3*learn_clk.getSeconds()) + .09);
					
					++db;
				}
			}
			
			gen.do_iteration();
			plotGen();
			
			
			clk.restart();
			
			GuiWindow &win = (GuiWindow &)getOwnerContext();
			win.setTitle("tanítások száma: " + fm::toString(db));
		}
	}
	
	void setPlot(Size i,const std::vector<vec2> &pts,vec4 c)
	{
		if (plots.size() <= i) plots.resize(i+1,nullptr);
		
		if (!plots[i]) {
			auto &cont = getOwnerContext();
			plots[i] = new Plot(cont.getSize(),cont);
	
			plots[i]->line(pts,c);
			
			cont.getMainLayout().addChildElement(plots[i]);
		}
		else {
			plots[i]->line(pts,c);
		}
	}
};



int main()
{
	GuiWindow win(vec2(640,480));
	win.setClearColor(vec4(.3,1));

	win.getMainLayout().addChildElement(new NNPlot(win.getSize(),win));
	
	win.runGuiLoop();
}
