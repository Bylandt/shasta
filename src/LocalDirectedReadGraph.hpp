#ifndef SHASTA_LOCAL_DIRECTED_READ_GRAPH_HPP
#define SHASTA_LOCAL_DIRECTED_READ_GRAPH_HPP




// Shasta.
#include "ReadId.hpp"

// Boost libraries.
#include <boost/graph/adjacency_list.hpp>

// Standard libraries.
#include <map>

namespace shasta {

    // Forward declaration of types declared in this file.
    class LocalDirectedReadGraphVertex;
    class LocalDirectedReadGraphEdge;
    class LocalDirectedReadGraph;
    using LocalDirectedReadGraphBaseClass = boost::adjacency_list<
        boost::setS,
        boost::listS,
        boost::bidirectionalS,
        LocalDirectedReadGraphVertex,
        LocalDirectedReadGraphEdge
        >;
}


class shasta::LocalDirectedReadGraphVertex {
public:

    OrientedReadId orientedReadId;
    uint32_t orientedReadIdValue;   // Used as vertex id.

    // The number of markers in this read.
    uint32_t markerCount;

    // The distance of this vertex from the starting vertex.
    uint32_t distance;

    // Used for Blast annotations.
    string additionalToolTipText;

    LocalDirectedReadGraphVertex(
        OrientedReadId orientedReadId,
        uint32_t markerCount,
        uint32_t distance) :
        orientedReadId(orientedReadId),
        orientedReadIdValue(orientedReadId.getValue()),
        markerCount(markerCount),
        distance(distance)
        {}

};



class shasta::LocalDirectedReadGraphEdge {
public:

    // Twice the offset between centers.
    int twiceOffsetAtCenter;

    // The number of alignment markers in the alignment
    // that created this edge.
    uint32_t markerCount;

    LocalDirectedReadGraphEdge(
        int twiceOffsetAtCenter,
        uint32_t markerCount) :
        twiceOffsetAtCenter(twiceOffsetAtCenter),
        markerCount(markerCount)
        {}
};



class shasta::LocalDirectedReadGraph :
    public LocalDirectedReadGraphBaseClass {
public:

    void addVertex(
        OrientedReadId,
        uint32_t baseCount,
        uint32_t distance);

    void addEdge(
        OrientedReadId,
        OrientedReadId,
        int twiceOffsetAtCenter,
        uint32_t markerCount);

    // Find out if a vertex with a given OrientedReadId exists.
    bool vertexExists(OrientedReadId) const;

    // Get the distance of an existing vertex from the start vertex.
    uint32_t getDistance(OrientedReadId) const;

    // Write in Graphviz format.
    void write(ostream&, uint32_t maxDistance) const;
    void write(const string& fileName, uint32_t maxDistance) const;

private:

    // Map that gives the vertex corresponding to an OrientedReadId.
    std::map<OrientedReadId, vertex_descriptor> vertexMap;

    // Graphviz writer.
    class Writer {
    public:
        Writer(const LocalDirectedReadGraph&, uint32_t maxDistance);
        void operator()(ostream&) const;
        void operator()(ostream&, vertex_descriptor) const;
        void operator()(ostream&, edge_descriptor) const;
        const LocalDirectedReadGraph& graph;
        uint32_t maxDistance;
    };
};



#endif
