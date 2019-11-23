#include "../utility/utility.h"
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <type_traits>


class Dijkstra
{
	struct VertexProcessing;

	struct Comparator
	{
		const Array<VertexProcessing>& verticesProcessing;

		Comparator(const Array<VertexProcessing>& verticesProcessing)
			: verticesProcessing(verticesProcessing)
		{}

		bool operator()(const index_t& lhs, const index_t& rhs) const
		{
			return verticesProcessing[lhs].distanceFromSource < verticesProcessing[rhs].distanceFromSource;
		}
	};

	typedef std::set<index_t, Comparator> queue_t;

	struct VertexProcessing
	{
		static const unsigned infinity{std::numeric_limits<unsigned>::max()};

		unsigned distanceFromSource{infinity};
		queue_t::iterator it_queue;
	};

public:
	struct Vertex : public Graph::Vertex
	{
		index_t i_previousVertex;

		using Graph::Vertex::operator=;
	};

	Array<Vertex> vertices;
    index_t i_source;

	Dijkstra(const Graph& graph, index_t i_source)
		: vertices(std::begin(graph.vertices), std::end(graph.vertices)), i_source(i_source)
	{
		Array<VertexProcessing> verticesProcessing(std::size(vertices));
		Comparator comparator(verticesProcessing);
		queue_t queue(comparator);

		verticesProcessing[i_source].distanceFromSource = 0;
		verticesProcessing[i_source].it_queue = queue.insert(i_source).first;
		
		while (!std::empty(queue))
		{
			index_t i_vertex = *std::begin(queue);
			queue.erase(std::begin(queue));
			const Vertex& vertex(vertices[i_vertex]);
			const VertexProcessing& vertexProcessing(verticesProcessing[i_vertex]);

			for (Graph::Edge edge : vertex.edges)
			{
				const index_t i_neighbourVertex(edge.i_end);
				Vertex& neighbourVertex(vertices[i_neighbourVertex]);
				VertexProcessing& neighbourVertexProcessing(verticesProcessing[i_neighbourVertex]);

                if (vertexProcessing.distanceFromSource + edge.weight >= neighbourVertexProcessing.distanceFromSource)
                    continue;

				if (neighbourVertexProcessing.distanceFromSource != VertexProcessing::infinity)
					queue.erase(neighbourVertexProcessing.it_queue);
					
				neighbourVertexProcessing.distanceFromSource = vertexProcessing.distanceFromSource + edge.weight;
				neighbourVertexProcessing.it_queue = queue.insert(i_neighbourVertex).first;
				neighbourVertex.i_previousVertex = i_vertex;
			}
		}
	}

	std::list<index_t> pathTo(index_t i_sink) const
	{
		std::list<index_t> route;
		for (index_t i_vertex(i_sink); i_vertex != i_source; i_vertex = vertices[i_vertex].i_previousVertex)
			route.push_front(i_vertex);
		
		route.push_front(i_source);
		
		return route;
	}
};


#if 0
int main()
{
	Graph graph;
	graph.vertices.push_back({{{1, 1}, {2, 2}, {3, 3}}});
	graph.vertices.push_back({{{3, 1}}});
	graph.vertices.push_back({{{3, 3}}});
	graph.vertices.push_back({{}});

	Dijkstra dijkstra(graph, 0);
	std::cout << dijkstra.pathTo(3) << '\n';
}
#endif
