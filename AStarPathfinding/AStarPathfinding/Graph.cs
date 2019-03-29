using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace AStarPathfinding
{
    class Graph
    {
        // Fields
        private int[,] adjacencyMatrix;
        private List<Vertex> vertices;
        private PriorityQueue openList;
        private List<Vertex> closedList;
        private List<Vertex> obstacles;
        private Vertex currentVertex;
        private Vertex start;
        private int startIndex;
        private Vertex end;
        private int endIndex;
        private bool pathIsLit;

        public Vertex CurrentVertex { get { return currentVertex; } }
        public Vertex Start { get { return start; } }
        public Vertex End { get { return end; } }

        public List<Vertex> Vertices { get { return vertices; } }

        /// <summary>
        /// Constructor, sets up the graph and all of the vertices.
        /// </summary>
        public Graph(Vertex start, Vertex end)
        {
            openList = new PriorityQueue();
            closedList = new List<Vertex>();
            currentVertex = start;
            this.start = start;
            this.end = end;
            pathIsLit = false;
            vertices = new List<Vertex>();
            obstacles = new List<Vertex>();
            adjacencyMatrix = new int[100, 100];

            startIndex = start.Rect.Y / 5 + start.Rect.X / 50;
            endIndex = end.Rect.Y / 5 + end.Rect.X / 50;

            // Create a list of vertices, 50 by 50 tiles
            // Calculate their H-values
            for (int row = 0; row < 10; row++)
            {
                for (int col = 0; col < 10; col++)
                {
                    vertices.Add(new Vertex(row * Vertex.TileSize, col * Vertex.TileSize));

                    // calculate H-Value = dx + dy
                    vertices[row * 10 + col].HValue = 
                        Math.Abs(end.Rect.X - vertices[row * 10 + col].Rect.X) +
                        Math.Abs(end.Rect.Y - vertices[row * 10 + col].Rect.Y);
                }
            }

            // Create the adjecency matrix for the 10x10 grid
            for(int r = 0; r < 100; r++)
            {
                for(int c = 0; c < 100; c++)
                {
                    adjacencyMatrix[r, c] = 0;
                }
            }

            for (int i = 0; i < 100; i++)
            {
                if(i % 10 == 0)                      // left side
                {
                    adjacencyMatrix[i, i + 1] = 1;

                    if (i < 90) // not a top side
                    {
                        adjacencyMatrix[i, i + 10] = 1;
                    }
                    if(i >= 10) // not a bottom side
                    {
                        adjacencyMatrix[i, i - 10] = 1;
                    }
                }
                else if(i % 10 == 9)                 // right side
                {
                    adjacencyMatrix[i, i - 1] = 1;

                    if (i < 90) // not a top side
                    {
                        adjacencyMatrix[i, i + 10] = 1;
                    }
                    if (i >= 10) // not a bottom side
                    {
                        adjacencyMatrix[i, i - 10] = 1;
                    }
                }
                else                                // neither
                {
                    adjacencyMatrix[i, i + 1] = 1;
                    adjacencyMatrix[i, i - 1] = 1;

                    if (i < 90) // not a bottom side
                    {
                        adjacencyMatrix[i, i + 10] = 1;
                    }
                    if (i >= 10) // not a top side
                    {
                        adjacencyMatrix[i, i - 10] = 1;
                    }
                }
            }

            // Set up the obstacles
            obstacles.Add(new Vertex(4, 9));
            obstacles.Add(new Vertex(4, 8));
            obstacles.Add(new Vertex(4, 7));
            obstacles.Add(new Vertex(4, 6));
            obstacles.Add(new Vertex(4, 5));
            obstacles.Add(new Vertex(5, 5));          
            obstacles.Add(new Vertex(6, 5));

            foreach (Vertex v in obstacles)
            {
                for(int i = 0; i < 100; i++)
                {
                    adjacencyMatrix[v.Rect.Y * 10 + v.Rect.X, i] = 0;
                    adjacencyMatrix[i, v.Rect.Y * 10 + v.Rect.X] = 0;
                }              
            }

            // Set up the neighbors
            for(int i = 0; i < 100; i++)
            {
                for (int j = 0; j < 100; j++)
                {
                    if(adjacencyMatrix[i,j] == 1)
                    {
                        vertices[i].AddNeighbor(vertices[j]);
                    }
                }
            }

            // Set up the start and end points within the vertices list
            vertices[startIndex].Texture = "start";
            vertices[startIndex].GValue = 0;
            vertices[endIndex].Texture = "end";
            openList.Enqueue(vertices[startIndex]);
        }
        

        /// <summary>
        /// Algorithm for A*
        /// </summary>
        public void AStarAlgorithm()
        {
            if(openList.Peek().Texture != end.Texture)
            {
                currentVertex = openList.Dequeue();
                closedList.Add(currentVertex);

                foreach(Vertex neighbor in currentVertex.Neighbors)
                {
                    int cost = currentVertex.GValue + 1;

                    if(openList.Contains(neighbor) && cost < neighbor.GValue)
                    {
                        openList.Remove(neighbor);
                    }

                    if(closedList.Contains(neighbor) && cost < neighbor.GValue)
                    {
                        closedList.Remove(neighbor);
                    }
                    // If it's not on either list, update its cost add it to the open list
                    if (!openList.Contains(neighbor) && !closedList.Contains(neighbor))
                    {
                        neighbor.GValue = cost;
                        openList.Enqueue(neighbor);  // priority = G(neighbor) + H(neighbor)
                        neighbor.Parent = currentVertex;
                    }
                }
            }
            else
            {
                ShortestPath();
            }

            if(!pathIsLit)
            {
                SetTextures();
            }            
        }

        /// <summary>
        /// Highlights the shortest path by following each vertex's parent starting at the end.
        /// </summary>
        public void ShortestPath()
        {           
            currentVertex = vertices[endIndex].Parent;

            while (currentVertex != vertices[startIndex])
            {
                currentVertex.Texture = "path";
                currentVertex = currentVertex.Parent;
            }

            pathIsLit = true;
        }

        /// <summary>
        /// Sets the textures of the vertices in the open and closed lists.
        /// </summary>
        public void SetTextures()
        {
            foreach (Vertex v in openList.List)
            {
                v.Texture = "open";
            }

            foreach (Vertex v in closedList)
            {
                v.Texture = "closed";
            }

            foreach(Vertex v in obstacles)
            {
                vertices[v.Rect.Y * 10 + v.Rect.X].Texture = "obstacle";
            }

            vertices[startIndex].Texture = "start";
            vertices[endIndex].Texture = "end";
        }
    }  
}
