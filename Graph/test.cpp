#include<iostream>
#include"Matrix.h"
#include"Graph.h"
#include<fstream>
namespace Khellendros
{
	//���Ժ���
	void testGraph(AbstractGraph<int>& graph)
	{
		using namespace std;
		cout << graph._edgeMatrix << endl;
		cout << graph._numOfWeight << endl;
	}
}
void makeGraph(Khellendros::AbstractGraph<int>& graph)
{
	std::ifstream fin("records.txt");
	if (!fin.is_open())
	{
		std::cout << "open file failed." << std::endl;
	}
	graph.create(fin);
}
int main()
{
	using namespace std;
	using namespace Khellendros;
	using namespace std::placeholders;
	typedef DirGraph<int> Graph;
	Graph graph;
	cout << "ͨ���ļ�����ͼ" << endl;
	makeGraph(graph);
	cout << "DFS ����" << endl;
	graph.forEach(Graph::DFS,
		[](int* plhs,int* pweight){
		Graph::printVertex(plhs, pweight, cout);
	});
	cout << "BFS ����" << endl;
	graph.forEach(Graph::BFS,
		[](int* plhs, int* pweight) {
		Graph::printVertex(plhs, pweight, cout);
	});
	system("pause");
	return 0;
}