using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Human class. Evades zombies (goblins in this case) and wanders when not fleeing.
/// </summary>
public class Human : Vehicle
{
    // Fields
    public GameObject closestZombie;
    public Vector3 targetLocation;
    public GameObject target;
    private const int fleeRadius = 100;
    public List<GameObject> humans;
    public List<GameObject> zombies;

    // Debug lines
    public Material material1;
    public Material material2;


    new void Start()
    { 
        base.Start();
        radius = transform.localScale.x + 1;
    }

    new void Update()
    {
        if(zombies.Count > 0)
        {
            SetClosestZombie();
        }       

        base.Update();
    }


    /// <summary>
    /// Calculates the ultimate steering force based on every force acting on the vehicle.
    /// </summary>
    public override void CalcSteeringForces()
    {
        Vector3 ultimateForce = Vector3.zero;

        // Flee from the closest zombie within 10 units
        if (closestZombie != null && (closestZombie.GetComponent<Zombie>().vehiclePosition - vehiclePosition).sqrMagnitude <= fleeRadius)
        {
            if((closestZombie.GetComponent<Zombie>().vehiclePosition - vehiclePosition).sqrMagnitude <= fleeRadius / 3)
            {
                ultimateForce += Flee(closestZombie) * 5;
            }
            else
            {
                ultimateForce += Evade(closestZombie) * 5;
            }            
        }
        else // Wander around the park
        {
            ultimateForce += Wander() * 0.5f;
        }

        foreach(GameObject obstacle in obstacles)
        {
            ultimateForce += ObstacleAvoidance(obstacle) * 2;
        }

        // Separate from nearby humans
        ultimateForce += Separation(humans, 2.5f) * 3;

        // Set up and apply the ultimate force
        ultimateForce.y = 0;
        ultimateForce.Normalize();
        ultimateForce *= maxForce;
        ApplyForce(ultimateForce);
    }


    /// <summary>
    /// Determines the closest zombie to the human.
    /// </summary>
    void SetClosestZombie()
    {
        float smallestDistance = 10000;
        float distance;
        int index = 0;

        for (int i = 0; i < zombies.Count; i++)
        {
            distance = Mathf.Abs(vehiclePosition.sqrMagnitude - zombies[i].GetComponent<Zombie>().vehiclePosition.sqrMagnitude);
            if (distance < smallestDistance)
            {
                smallestDistance = distance;
                index = i;
            }
        }
        closestZombie = zombies[index];
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

            // Draw the sphere at the future position
            
        }
    }
}
