#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>

using namespace std;

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph {
private:

    // typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

// ----------------------------------------------------------------
//  Description:    An array of all the nodes in the graph.
// ----------------------------------------------------------------
    Node** m_pNodes;

// ----------------------------------------------------------------
//  Description:    The maximum number of nodes in the graph.
// ----------------------------------------------------------------
    int m_maxNodes;


// ----------------------------------------------------------------
//  Description:    The actual number of nodes in the graph.
// ----------------------------------------------------------------
    int m_count;


public:           
    // Constructor and destructor functions
    Graph( int size );
    ~Graph();

    // Accessors
    Node** nodeArray() const {
       return m_pNodes;
    }

	class NodeSearchCostComparer {
	public:
		bool operator()(Node * n1, Node * n2) {
			tuple<string, int, int, int, int> p1 = n1->data();
			tuple<string, int, int, int, int> p2 = n2->data();
			//int p1a = get<1>(p1) +get<2>(p1);
			//int p2a = get<1>(p2) +get<2>(p2);
			return get<1>(p1) +get<2>(p1) > get<1>(p2) +get<2>(p2);
		}
	};


    // Public member functions.
    bool addNode( NodeType data, int index );
    void removeNode( int index );
    bool addArc( int from, int to, ArcType weight );
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );        
    void clearMarks();
    void depthFirst( Node* pNode, void (*pProcess)(Node*) );
    void breadthFirst( Node* pNode, void (*pProcess)(Node*) );
	void adaptedBreadthFirst( Node* pCurrent, Node* pGoal );
	void ucs(Node* pStart, Node* pDest, void(*pVisitFunc)(Node*),std::vector<Node *>& path);
	void aStar(Node* pStart, Node* pDest, void(*pProcess)(Node*),std::vector<Node *>& path, int);
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph( int size ) : m_maxNodes( size ) {
   int i;
   m_pNodes = new Node * [m_maxNodes];
   // go through every index and clear it to null (0)
   for( i = 0; i < m_maxNodes; i++ ) {
        m_pNodes[i] = 0;
   }

   // set the node count to 0.
   m_count = 0;
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
   int index;
   for( index = 0; index < m_maxNodes; index++ ) {
        if( m_pNodes[index] != 0 ) {
            delete m_pNodes[index];
        }
   }
   // Delete the actual array
   delete m_pNodes;
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode( NodeType data, int index ) {
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( m_pNodes[index] == 0) {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
      m_pNodes[index] = new Node;
      m_pNodes[index]->setData(data);
      m_pNodes[index]->setMarked(false);

      // increase the count and return success.
      m_count++;
    }
        
    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode( int index ) {
     // Only proceed if node does exist.
     if( m_pNodes[index] != 0 ) {
         // now find every arc that points to the node that
         // is being removed and remove it.
         int node;
         Arc* arc;

         // loop through every node
         for( node = 0; node < m_maxNodes; node++ ) {
              // if the node is valid...
              if( m_pNodes[node] != 0 ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_pNodes[node]->getArc( m_pNodes[index] );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_pNodes[index];
        m_pNodes[index] = 0;
        m_count--;
    }
}

// ----------------------------------------------------------------
//  Name:           addArd
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) {
     bool proceed = true; 
     // make sure both nodes exist.
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     //if( m_pNodes[from]->getArc( m_pNodes[to] ) != 0 ) {
     //    proceed = false;
     //}

     if (proceed == true) {
        // add the arc to the "from" node.
        m_pNodes[from]->addArc( m_pNodes[to], weight );
     }
        
     return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc( int from, int to ) {
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         nodeExists = false;
     }

     if (nodeExists == true) {
        // remove the arc.
        m_pNodes[from]->removeArc( m_pNodes[to] );
     }
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
// Dev-CPP doesn't like Arc* as the (typedef'd) return type?
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc( int from, int to ) {
     Arc* pArc = 0;
     // make sure the to and from nodes exist
     if( m_pNodes[from] != 0 && m_pNodes[to] != 0 ) {
         pArc = m_pNodes[from]->getArc( m_pNodes[to] );
     }
                
     return pArc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() {
     int index;
     for( index = 0; index < m_maxNodes; index++ ) {
          if( m_pNodes[index] != 0 ) {
              m_pNodes[index]->setMarked(false);
          }
     }
}


// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst( Node* pNode, void (*pProcess)(Node*) ) {
     if( pNode != 0 ) {
           // process the current node and mark it
           pProcess( pNode );
           pNode->setMarked(true);

           // go through each connecting node
           list<Arc>::iterator iter = pNode->arcList().begin();
           list<Arc>::iterator endIter = pNode->arcList().end();
        
		   for( ; iter != endIter; ++iter) {
			    // process the linked node if it isn't already marked.
                if ( (*iter).node()->marked() == false ) {
                   depthFirst( (*iter).node(), pProcess);
                }            
           }
     }
}


// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a depth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst( Node* pNode, void (*pProcess)(Node*) ) {
   if( pNode != 0 ) {
	  queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( pNode );
      pNode->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 ) {
         // process the node at the front of the queue.
         pProcess( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) {
              if ( (*iter).node()->marked() == false) {
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}


// ----------------------------------------------------------------
//  Name:           adaptedBreadthFirst
//  Description:    Performs a breadth-first traversal the starting node
//                  specified as an input parameter, terminating at the goal.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::adaptedBreadthFirst( Node* pCurrent, Node *pGoal ) {
   if( pCurrent != 0 && pGoal != 0 ) {
	  bool goalReached = false;
	  queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( pCurrent );
      pCurrent->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 && goalReached == false) {
         // process the node at the front of the queue.
         // pProcess( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter && goalReached == false; iter++ ) {
			  // Check if current child node == goal
			  if ( (*iter).node() == pGoal ) {
				goalReached = true;
				pGoal->setPrevious( (*iter).node() );
			  }
              if ( (*iter).node()->marked() == false) {
				 // Set previous pointer of child to current.
				 (*iter).node()->setPrevious( nodeQueue.front() );
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::ucs(Node* pStart, Node* pDest, void(*pVisitFunc)(Node*), std::vector<Node *>& path){
	
	//Let s = the starting node, g = goal or destination node
	//	Let pq = a new priority queue
	priority_queue<Node *, vector<Node*>, NodeSearchCostComparer> pq;
	
	//	Initialise d[s] to 0
	pStart->setData(pair<string,int>(pStart->data().first,0));
	//	For each node v in graph G
	//	Initialise d[v] to infinity // don’t yet know the distances to these nodes
	//	Add s to the pq
	pq.push(pStart);
	//	Mark(s)
	pStart->setMarked(true);
	while (pq.size() != 0 && pq.top() != pDest) {
		list<Arc>::const_iterator iter = pq.top()->arcList().begin();
		list<Arc>::const_iterator endIter = pq.top()->arcList().end();

		for (; iter != endIter; iter++) {
			if ((*iter).node() != pq.top()->previous()){

				int distC = (*iter).weight() + pq.top()->data().second;

				if (distC < (*iter).node()->data().second)
				{
					(*iter).node()->setData(pair<string, int>((*iter).node()->data().first, distC));

					(*iter).node()->setPrevious(pq.top());
				}

				
				if ((*iter).node()->marked() == false) {
					pq.push((*iter).node());
					(*iter).node()->setMarked(true);
				}
			}
		}
		pq.pop();			
					
	}
	for (auto node = pDest; node->previous() != 0; node = node->previous())
	{
		path.push_back(node);
	}
	path.push_back(pStart);

}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::aStar(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node *>& path, int max) {

	priority_queue<Node *, vector<Node*>, NodeSearchCostComparer> pq;

	pq.push(pStart);
	pStart->setMarked(true);
	pStart->setData(NodeType(get<0>(pStart->data()), 0, get<2>(pStart->data()), get<3>(pStart->data()), get<4>(pStart->data())));
	float gx = get<3>(pDest->data());
	float gy = get<4>(pDest->data());

	for (int i = 0; i < max; i++) {
		int x = get<3>(nodeArray()[i]->data());
		int y = get<4>(nodeArray()[i]->data());
		//int dist = 10;
		int dist = sqrt(((gx-x)*(gx-x))+((gy-y)*(gy-y)));
		nodeArray()[i]->setData(NodeType(get<0>(nodeArray()[i]->data()), get<1>(nodeArray()[i]->data()), dist, get<3>(nodeArray()[i]->data()), get<4>(nodeArray()[i]->data())));
	}

	while (pq.size() != 0 && pq.top() != pDest) {
		list<Arc>::const_iterator iter = pq.top()->arcList().begin();
		list<Arc>::const_iterator endIter = pq.top()->arcList().end();

		for (; iter != endIter; iter++) {

			if ((*iter).node() != pq.top()->previous()) {//&&

				int gN = get<2>(pq.top()->data()) + (*iter).weight();
				int hN = get<1>(pq.top()->data());
				int distC = gN + hN;

				int fC = get<1>((*iter).node()->data()) + get<2>((*iter).node()->data());
				if (distC < fC) {

					(*iter).node()->setData(NodeType(get<0>((*iter).node()->data()), gN, get<2>((*iter).node()->data()), get<3>((*iter).node()->data()), get<4>((*iter).node()->data())));
					(*iter).node()->setPrevious(pq.top());
				}
				if ((*iter).node()->marked() != true) {

					pq.push((*iter).node());
					(*iter).node()->setMarked(true);
				}
			}
		}
		pq.pop();
	}
	for (auto node = pDest; node->previous() != 0; node = node->previous())
	{
		path.push_back(node);
	}
	path.push_back(pStart);

}


#include "GraphNode.h"
#include "GraphArc.h"

#endif

