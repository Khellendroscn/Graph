#pragma once
#include "Matrix.h"
#include <vector>
#include <memory>
#include <set>
#include <functional>
#include <queue>
#include <iostream>
#include <string>
#include <sstream>

namespace Khellendros 
{
	template<typename T,typename E = std::int32_t>
	class AbstractGraph
	{
	public:
		typedef typename T ValueType;//����ֵ����
		typedef typename E WeightType;//Ȩ����
		typedef std::size_t SizeType;
		//�ص���������
		typedef typename std::function<void(ValueType*,WeightType*)> BackCallFunc;
		//������������
		typedef typename std::function<void(AbstractGraph&, BackCallFunc)> TravelFunc;

		const static SizeType DefaultSize = 10;

		friend void testGraph(AbstractGraph& graph);

		AbstractGraph(SizeType size) :
			_maxSize(size),
			_numOfWeight(0),
			_edgeMatrix(size,size,0)
		{}

		AbstractGraph() :
			_maxSize(DefaultSize),
			_numOfWeight(0), 
			_edgeMatrix(DefaultSize,DefaultSize,0)
		{}

		AbstractGraph(const std::initializer_list<ValueType>& initList) :
			_maxSize(initList.size()),
			_numOfWeight(0), 
			_vertexList(initList),
			_edgeMatrix(initList.size(),initList.size(), 0) 
		{}
		
		virtual ~AbstractGraph() = 0{}
		void swap(AbstractGraph& graph)
		{
			swap(_vertexList, graph._vertexList);
			swap(_edgeMatrix, graph._edgeMatrix);
		}

		SizeType getNumOfVertex()const { return _vertexList.size(); }
		SizeType getNumOfWeight()const { return _numOfWeight; }
		WeightType& getWeight(int lhsIndex, int rhsIndex) { return _edgeMatrix[lhsIndex][rhsIndex]; }
		void insert(const ValueType& vertex)
		{
			//����һ������
			if (_vertexList.size() >= _maxSize)
			{
				//������󲻹��󣬹���һ������ľ���
				_maxSize *= 2;
				Matrix<WeightType> temp(_maxSize, _maxSize,0);
				temp.move(std::move(_edgeMatrix));//�ƶ�����
				Khellendros::swap(_edgeMatrix, temp);
			}
			_vertexList.push_back(vertex);
		}
		void insertEdge(SizeType lhsIndex, SizeType rhsIndex, WeightType weight)//������������
		{
			if (lhsIndex >= _vertexList.size() || rhsIndex >= _vertexList.size())
			{
				throw std::logic_error("Index out of range");
				return;
			}
			doSetEdge(lhsIndex, rhsIndex, weight);
			++_numOfWeight;
		}
		void removeEdge(SizeType lhsIndex, SizeType rhsIndex)//�Ͽ���������
		{
			if (lhsIndex >= _vertexList.size() || rhsIndex >= _vertexList.size())
			{
				throw std::logic_error("Index out of range");
				return;
			}
			doSetEdge(lhsIndex, rhsIndex, 0);
			--_numOfWeight;
		}
		int getNeighbor(int index,int nextNeighbor=0)
		{
			int i = nextNeighbor+1;
			for (; i < _vertexList.size(); ++i)
				if (_edgeMatrix[index][i] != 0) break;
			return i >= _vertexList.size() ? -1 : i;
		}
		void forEach(TravelFunc travelFunc, BackCallFunc backcall)
		{
			travelFunc(*this, backcall);
		}

		static void DFS(AbstractGraph& g, BackCallFunc backcall)
		{
			std::set<int> visitedSet;//�ѱ������±�ļ���
			for (int i = 0; i < g.getNumOfVertex(); ++i)
			{
				if (visitedSet.find(i) == visitedSet.end())
				{
					//����±�δ������
					visitedSet.insert(i);//�����±���ӵ�������
					g.doDFS(i, visitedSet, backcall);
				}
			}
		}
		static void BFS(AbstractGraph& g, BackCallFunc backcall)
		{
			std::set<int> visitedSet;
			std::queue<int> travelQueue;
			for (int i = 0; i < g.getNumOfVertex(); ++i)
			{
				if (visitedSet.find(i) == visitedSet.end())
				{
					//����±�δ������
					visitedSet.insert(i);//�����±���ӵ�������
					travelQueue.push(i);
					while (!travelQueue.empty())
					{
						int index = travelQueue.front();
						travelQueue.pop();
						int neighbor = g.getNeighbor(index);
						while (neighbor >= 0 && visitedSet.find(neighbor) != visitedSet.end())
							neighbor = g.getNeighbor(index, neighbor);
						for (; neighbor >= 0 && neighbor < g._vertexList.size(); neighbor = g.getNeighbor(index, neighbor))
						{
							visitedSet.insert(neighbor);
							WeightType weight = g.getWeight(index, neighbor);
							backcall(&g._vertexList[index], &weight);
							backcall(&g._vertexList[neighbor], nullptr);
							travelQueue.push(neighbor);
						}
					}
				}
			}
		}
		SizeType& operator[](int index)
		{
			return _vertexList[index];
		}
		SizeType operator[](int index)const
		{
			return _vertexList[index];
		}
		template<typename T,typename E>
		friend std::istream& operator >> (std::istream& is, AbstractGraph<T,E>& graph);
		void create(std::istream& is)
		{
			std::string temp;
			for (std::getline(is, temp); temp.size() > 1; std::getline(is, temp))
			{
				std::cout << temp << std::endl;
				std::istringstream sin(temp);
				sin >> *this;
			}
		}
	protected:
		Matrix<WeightType>& getEdgeMatrix() { return _edgeMatrix; }
	private:
		std::vector<ValueType> _vertexList;//�����б�
		Matrix<WeightType> _edgeMatrix;//�ڽӾ���
		SizeType _maxSize;
		SizeType _numOfWeight;

		virtual void doSetEdge(SizeType lhsIndex, SizeType rhsIndex, WeightType weight) = 0;

		void doDFS(int index,std::set<int>& visitedSet ,BackCallFunc backcall)
		{
			//�����������

			int neighbor = getNeighbor(index);//��ȡ��һ���ڽӶ���
			while (neighbor>=0&&visitedSet.find(neighbor)!=visitedSet.end())
			{
				//Ѱ�ҵ�һ��δ���ʵ��ڽӶ���
				neighbor = getNeighbor(index, neighbor);
			}
			if (neighbor < 0) 
			{
				//���û���ھ�
				backcall(&_vertexList[index], nullptr); 
			}
			else
			{
				for (; neighbor >= 0&&neighbor<_vertexList.size(); neighbor = getNeighbor(index, neighbor))
				{
					//������ȱ���index�������ھ�
					if (visitedSet.find(neighbor) == visitedSet.end())
					{
						//std::cout << "( " << index << " , " << neighbor << " )" << std::endl;
						visitedSet.insert(neighbor);
						WeightType weight = getWeight(index, neighbor);
						backcall(&_vertexList[index], &weight);
						doDFS(neighbor, visitedSet, backcall);
					}
				}
			}
		}
	};
	template<typename T>
	void swap(AbstractGraph<T>& lhs, AbstractGraph<T>& rhs)
	{
		lhs.swap(rhs);
	}
	template<typename T, typename E>
	std::istream& operator >> (std::istream& is, AbstractGraph<T, E>& graph)
	{
		T lhs, rhs;
		E weight;
		is >> lhs >> rhs >> weight;
		if(graph._vertexList.empty()||lhs!=graph._vertexList.back()) graph.insert(lhs);
		graph.insert(rhs);
		graph.insertEdge(graph._vertexList.size() - 2, graph._vertexList.size() - 1, weight);
		return is;
	}

	template<typename T,typename E = std::int32_t>
	class UndirGraph :public AbstractGraph<T, E>
	{
	public:
		typedef typename T ValueType;
		typedef typename E WeightType;
		typedef typename  AbstractGraph<T, E> Base;
		using Base::SizeType;
		using Base::DefaultSize;

		UndirGraph() = default;
		UndirGraph(SizeType size) :Base(size) {}
		UndirGraph(const std::initializer_list<ValueType>& initList) :Base(initList) {}

		static void printVertex(ValueType* plhs, WeightType* pweight, std::ostream& os)
		{
			os << *plhs;
			if (pweight)
			{
				os << " --( " << *pweight << " )-- ";
			}
			else
			{
				os << std::endl;
			}
		}
	private:
		void doSetEdge(SizeType lhsIndex, SizeType rhsIndex, WeightType weight)override
		{
			auto& edges = getEdgeMatrix();
			edges[lhsIndex][rhsIndex] = weight;
			edges[rhsIndex][lhsIndex] = weight;
		}
	};

	template<typename T, typename E = std::int32_t>
	class DirGraph :public AbstractGraph<T, E>
	{
	public:
		typedef typename T ValueType;
		typedef typename E WeightType;
		typedef typename  AbstractGraph<T, E> Base;
		using Base::SizeType;
		using Base::DefaultSize;

		DirGraph() = default;
		DirGraph(SizeType size) :Base(size) {}
		DirGraph(const std::initializer_list<ValueType>& initList) :Base(initList) {}

		static void printVertex(ValueType* plhs, WeightType* pweight, std::ostream& os)
		{
			os << *plhs;
			if (pweight)
			{
				os << " --( " << *pweight << " )-> ";
			}
			else
			{
				os << std::endl;
			}
		}
	private:
		void doSetEdge(SizeType lhsIndex, SizeType rhsIndex, WeightType weight)override
		{
			auto& edges = getEdgeMatrix();
			edges[lhsIndex][rhsIndex] = weight;
		}
	};
}