using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Zombie class, Pursues the closest human and wanders when there are no humans to pursue.
/// </summary>
public class Zombie : Vehicle
{
    // Fields
    public GameObject closestHuman;
    public List<GameObject> humans;
    public List<GameObject> zombies;
    //public float zombieRadius;  // Base this on 'agility'

    // Debug lines
    public Material material1;
    public Material material2;
    public Material material3;


    new void Start()
    {
        base.Start();
        radius = transform.localScale.x + 1;
    }

    new void Update()
    {
        if(humans.Count > 0)
        {
            SetClosestHuman();
        }

        base.Update();
    }


    /// <summary>
    /// Calculates the ultimate steering force based on every force acting on the vehicle.
    /// </summary>
    public override void CalcSteeringForces()
    {
        Vector3 ultimateForce = Vector3.zero;

        // Seek the closest human.
        if(humans.Count > 0)
        {
            if((vehiclePosition - closestHuman.GetComponent<Vehicle>().vehiclePosition).sqrMagnitude > 16)
            {
                ultimateForce += Pursue(closestHuman) * 2;
            }
            else
            {
                ultimateForce += Seek(closestHuman) * 2;
            }               
        }

        // Avoid obstacles.
        foreach (GameObject obstacle in obstacles)
        {
            ultimateForce += ObstacleAvoidance(obstacle) * 10;
        }

        // Separate from nearby zombies
        ultimateForce += Separation(zombies, 5) * 10;

        // Stop moving if there are no more humans.
        if (humans.Count <= 0)
        {
            ultimateForce += Wander() * 0.5f;
        }

        // Set up and apply the ultimate force.
        ultimateForce.y = 0;
        ultimateForce.Normalize();
        ultimateForce *= maxForce;
        ApplyForce(ultimateForce);   
    }


    /// <summary>
    /// Determines the closest human to the zombie.
    /// </summary>
    void SetClosestHuman()
    {
        float smallestDistance = 10000;
        float distance;
        int index = 0;

        for (int i = 0; i < humans.Count; i++)
        {
            // using sqrMagnitude since the distances are relative
            distance = (vehiclePosition - humans[i].GetComponent<Human>().vehiclePosition).sqrMagnitude;
            if (distance < smallestDistance)
            {
                smallestDistance = distance;
                index = i;
            }
        }
        closestHuman = humans[index];
    }


    /// <summary>
    /// 'D' key toggles debug lines being on or off.
    /// </summary>
    void OnRenderObject()
    {
        if (debugLinesOn)
        {
            // Draw the forward vector
            material1.SetPass(0);

            GL.Begin(GL.LINES);
            GL.Vertex(vehiclePosition);
            GL.Vertex(vehiclePosition + transform.forward * 2);
            GL.End();

            // Draw the right vector
            material2.SetPass(0);

            GL.Begin(GL.LINES);
            GL.Vertex(vehiclePosition);
            GL.Vertex(vehiclePosition + transform.right * 2);
            GL.End();

            // Draw a line to the closest human
            if(humans.Count > 0)
            {
                material3.SetPass(0);

                GL.Begin(GL.LINES);
                GL.Vertex(vehiclePosition);
                GL.Vertex(closestHuman.GetComponent<Human>().vehiclePosition);
                GL.End();
            }           
        }
    }
}
