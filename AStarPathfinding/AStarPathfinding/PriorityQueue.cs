using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AStarPathfinding
{
    class PriorityQueue
    {
        // Field
        private List<Vertex> list;
        
        public int Count { get { return list.Count; } }
        public List<Vertex> List { get { return list; } }

        /// <summary>
        /// Constructor.
        /// </summary>
        public PriorityQueue()
        {
            list = new List<Vertex>();
        }


        // Methods
        /// <summary>
        /// Adds new data to the queue (and the binary heap).
        /// </summary>
        /// <param name="data"></param>
        public void Enqueue(Vertex data)
        {
            list.Add(data);

            if (list.Count > 1)
            {
                int index = list.IndexOf(data);
                int parentIndex = (index - 1) / 2;

                while (list[parentIndex].FValue > list[index].FValue)
                {
                    Swap(index, parentIndex);
                    index = parentIndex;  // updates the current index to where the data is
                    parentIndex = (index - 1) / 2;  // finds the next parent index
                }
            }
        }

        /// <summary>
        /// Removes and returns the highest priority (smallest number) in the queue/heap.
        /// </summary>
        /// <returns>Returns what is removed.</returns>
        public Vertex Dequeue()
        {
            Vertex value = list[0];  // store the value of the current root to return later
            list[0] = list[list.Count - 1];  // set the root equal to the last element
            list.RemoveAt(list.Count - 1);  // remove the last element (it's now the root)
            int index = 0;
            int child1Index = 1;
            int child2Index = 2;

            while (ChildIndexCheck(index, child1Index) || ChildIndexCheck(index, child2Index))
            {
                if (ChildIndexCheck(index, child1Index) && ChildIndexCheck(index, child2Index))  // If the index's value is bigger than both child values
                {
                    if (list[child1Index].FValue < list[child2Index].FValue)  // swap with the smallest, update the current index
                    {
                        Swap(index, child1Index);
                        index = child1Index;
                    }
                    else
                    {
                        Swap(index, child2Index);
                        index = child2Index;
                    }
                }
                else if (child1Index < list.Count && list[index].FValue > list[child1Index].FValue)
                {
                    Swap(index, child1Index);
                    index = child1Index;
                }
                else if (child2Index < list.Count)
                {
                    Swap(index, child2Index);
                    index = child2Index;
                }

                child1Index = 2 * index + 1;
                child2Index = 2 * index + 2;
            }

            return value;
        }

        /// <summary>
        /// Returns (but does not remove) the first piece of data in the List.
        /// </summary>
        /// <returns>The first piece of data in the list.</returns>
        public Vertex Peek()
        {
            return list[0];
        }

        /// <summary>
        /// Determines if the priority queue is empty.
        /// </summary>
        /// <returns>True if it is empty, false otherwise.</returns>
        public bool IsEmpty()
        {
            if (list.Count == 0)
                return true;
            return false;
        }

        /// <summary>
        /// Swaps the values of two indices.
        /// </summary>
        /// <param name="index1"></param>
        /// <param name="index2"></param>
        private void Swap(int index1, int index2)
        {
            Vertex temp = list[index1];
            list[index1] = list[index2];
            list[index2] = temp;
        }

        /// <summary>
        /// Checks to see if the child index is within the list and if its value is less than that of the current index's.
        /// </summary>
        /// <param name="index"></param>
        /// <param name="childIndex"></param>
        /// <returns>True if the child index is within the list and if its value is less than that of the current index's, false otherwise.</returns>
        private bool ChildIndexCheck(int index, int childIndex)
        {
            if (childIndex >= list.Count)
            {
                return false;
            }

            if (list[index].FValue <= list[childIndex].FValue)
            {
                return false;
            }

            return true;
        }

        /// <summary>
        /// The Contains() method for the priorirt queue.
        /// </summary>
        /// <param name="v"></param>
        /// <returns></returns>
        public bool Contains(Vertex v)
        {
            if (list.Contains(v))
                return true;
            return false;
        }

        /// <summary>
        /// Remove method.
        /// </summary>
        /// <param name="v"></param>
        public void Remove(Vertex v)
        {
            list.Remove(v);
        }
    }
}
