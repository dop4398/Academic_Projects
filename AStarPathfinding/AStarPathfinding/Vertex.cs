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
    class Vertex
    {
        // Fields and properties
        private bool visited;
        private Rectangle rect;
        private static int tileSize = 50;
        private Vertex parent;
        private string texture;
        private int hValue;
        private int gValue;
        private List<Vertex> neighbors;

        public bool Visited
        {
            get { return visited; }
            set { visited = value; }
        }
        public Rectangle Rect { get { return rect; } }
        public static int TileSize { get { return tileSize; } }
        public Vertex Parent
        {
            get { return parent; }
            set { parent = value; }
        }
        public string Texture
        {
            get { return texture; }
            set { texture = value; }
        }
        public int GValue
        {
            get { return gValue; }
            set { gValue = value; }
        }
        public int HValue
        {
            get { return hValue; }
            set { hValue = value; }
        }
        public int FValue
        {
            get { return gValue + hValue; }
        }
        public List<Vertex> Neighbors
        {
            get { return neighbors; }
        }

        /// <summary>
        /// Constructor for the Vertex Class. Takes in the x and y coordinates for the vertex.
        /// </summary>
        public Vertex(int row, int col)
        {
            visited = false;
            rect = new Rectangle(col, row, tileSize, tileSize);
            Reset();
            neighbors = new List<Vertex>();
        }

        /// <summary>
        /// Resets the shortest distance and parent values.
        /// </summary>
        public void Reset()
        {
            gValue = int.MaxValue;
            parent = null;
        }

        public void AddNeighbor(Vertex neighbor)
        {
            neighbors.Add(neighbor);
        }
    }
}
