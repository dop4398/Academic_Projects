using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Vehicle class, base for Human and Zombie classes.
/// </summary>
 abstract public class Vehicle : MonoBehaviour
{
    // Movement vectors
    public Vector3 vehiclePosition;
    public Vector3 acceleration;
    public Vector3 direction;
    public Vector3 velocity;

    // other fields
    public float mass;
    protected float frictionCoeff;
    public float maxSpeed;
    public float floorWidth;
    public float floorLength;
    public float maxForce;
    public bool debugLinesOn;
    public GameObject debugPrefab;
    public GameObject debugSphere;

    // Obstacles to avoid
    public List<GameObject> obstacles;
    public float radius;
    public float safeDistance;

    // Wander variables
    public float wanderRadius;
    public int frameCount;
    public Vector3 wanderSpot;
    public Vector3 spot;


    public void Start ()
    {
        vehiclePosition = transform.position;
        frictionCoeff = 4f;
        floorWidth = 20;
        floorLength = 20;
        wanderRadius = 1;
        frameCount = 60;
        spot = Vector3.zero;

        // Debug sphere at the vehicle's future position
        debugSphere = Instantiate(debugPrefab, vehiclePosition, Quaternion.identity);
        debugSphere.GetComponent<Renderer>().enabled = false;
    }
	

	public void Update ()
    {
        // steering forces
        CalcSteeringForces();
        TurnAwayFromWall();

        // movement formula
        velocity += acceleration * Time.deltaTime;
        vehiclePosition += velocity * Time.deltaTime;
        direction = velocity.normalized;
        acceleration = Vector3.zero;
        transform.position = vehiclePosition;
        transform.forward = direction;

        // debug sphere update
        if(debugLinesOn)
        {
            debugSphere.GetComponent<Renderer>().enabled = true;
        }
        else
        {
            debugSphere.GetComponent<Renderer>().enabled = false;
        }
        debugSphere.transform.position = new Vector3(vehiclePosition.x + velocity.x, 1, vehiclePosition.z + velocity.z);
    }


    // Abstract method to be overridden by the Human and Zombie classes.
    abstract public void CalcSteeringForces();


    /// <summary>
    /// Receive an incoming force, divide by mass, and apply to the cumulative acceleration vector.
    /// </summary>
    /// <param name="force"></param>
    public void ApplyForce(Vector3 force)
    {
        acceleration += force / mass;
    }


    /// <summary>
    /// Move in a straight line toward a target
    /// </summary>
    /// <param name="targetLocation"></param>
    /// <returns></returns>
    public Vector3 Seek(GameObject target)
    {
        Vector3 desiredVelocity = target.transform.position - vehiclePosition;
        desiredVelocity.Normalize();
        desiredVelocity *= maxSpeed;

        Vector3 seekForce = desiredVelocity - velocity;

        return seekForce;
    }
    public Vector3 Seek(Vector3 target)
    {
        Vector3 desiredVelocity = target - vehiclePosition;
        desiredVelocity.Normalize();
        desiredVelocity *= maxSpeed;

        Vector3 seekForce = desiredVelocity - velocity;

        return seekForce;
    }


    public Vector3 Pursue(GameObject target)
    {
        Vector3 desiredVelocity = ((target.transform.position + target.GetComponent<Vehicle>().velocity) - vehiclePosition);
        desiredVelocity.Normalize();
        desiredVelocity *= maxSpeed;
        Vector3 seekForce = desiredVelocity - velocity;
        return seekForce;
    }


    /// <summary>
    /// Opposite(ish) of Seek
    /// Move in a straight line away from a target
    /// </summary>
    /// <param name="targetLocation"></param>
    /// <returns></returns>
    public Vector3 Flee(GameObject target)
    {
        Vector3 desiredVelocity = target.transform.position - vehiclePosition;
        desiredVelocity.Normalize();
        desiredVelocity *= maxSpeed;

        Vector3 fleeForce = desiredVelocity * -1 - velocity;

        return fleeForce;
    }

    public Vector3 Evade(GameObject target)
    {
        Vector3 desiredVelocity = ((target.transform.position + target.GetComponent<Vehicle>().velocity) - vehiclePosition).normalized * maxSpeed;
        Vector3 fleeForce = (desiredVelocity * -1) - velocity;
        return fleeForce;
    }


    /// <summary>
    /// Applies friction to the vehicle.
    /// Acts opposite to velocity.
    /// </summary>
    /// <param name="coeff"></param>
    public void ApplyFriction(float coeff)
    {
        Vector3 friction = velocity * -1;
        friction.Normalize();
        friction *= coeff;
        acceleration += friction;
    }


    /// <summary>
    /// Adds a force toward the center of the park whenever a vehicle gets too close to an edge.
    /// </summary>
    void TurnAwayFromWall()
    {
        // The -3s and +3s are to get the vehicles turning before going off the edges.
        if (vehiclePosition.x > floorWidth - 4 || vehiclePosition.x < -floorWidth + 4)
        {
            ApplyForce(new Vector3(0, vehiclePosition.y, 0) - 
                new Vector3(vehiclePosition.x * 1.1f, vehiclePosition.y, vehiclePosition.z * 1.1f));
        }

        if (vehiclePosition.z > floorLength - 4 || vehiclePosition.z < -floorLength + 4)
        {
            ApplyForce(new Vector3(0, vehiclePosition.y, 0) -
                 new Vector3(vehiclePosition.x * 1.1f, vehiclePosition.y, vehiclePosition.z * 1.1f));
        }
    }


    /// <summary>
    /// 
    /// </summary>
    /// <param name="obstacle"></param>
    /// <returns>Returns the force needed to avoid an obstacle.</returns>
    protected Vector3 ObstacleAvoidance(GameObject obstacle)
    {
        // Vector to obstacle
        Vector3 vecToCenter = obstacle.transform.position - vehiclePosition;
        // dot product to forward
        float dotForward = Vector3.Dot(vecToCenter, transform.forward);
        // dot product to right
        float dotRight = Vector3.Dot(vecToCenter, transform.right);
        // radii sum
        float radiiSum = obstacle.GetComponent<Obstacle>().radius + radius;

        // Step 1: what is behind? If so, return an empty force.
        if(dotForward < 0)
        {
            return Vector3.zero;
        }

        // Step 2: is it within distance? If not, return an empty force.
        if(vecToCenter.sqrMagnitude > (safeDistance * safeDistance))
        {
            return Vector3.zero;
        }

        // Step 3: test for non-intersections.
        if(radiiSum < Mathf.Abs(dotRight))
        {
            return Vector3.zero;
        }

        // Step 4: calculate the dodge force.
        Vector3 desiredVelocity;
        if(dotRight < 0) // Left
        {
            desiredVelocity = transform.right * maxSpeed;
        }
        else // Right
        {
            desiredVelocity = -transform.right * maxSpeed;
        }

        Debug.DrawLine(transform.position, obstacle.transform.position, Color.green);

        return desiredVelocity - velocity;
    }


    /// <summary>
    /// Gives objects of like types some personal space when moving together.
    /// </summary>
    /// <param name="friends">List of friends to steer away from.</param>
    /// <returns></returns>
    public Vector3 Separation(List<GameObject> friends, float separationDistance)
    {
        Vector3 penultimateForce = Vector3.zero;
        float sqrDistance;

        // Calling the list 'friends' because each object will only separate from others of its kind. (humans separate from other humans, etc.)
        foreach(GameObject friend in friends)
        {
            sqrDistance = (friend.GetComponent<Vehicle>().vehiclePosition - vehiclePosition).sqrMagnitude;
            // If the friends are within so many units of each other, push them apart.
            if (sqrDistance < separationDistance * separationDistance && friend.GetComponent<Vehicle>().vehiclePosition != vehiclePosition)
            {
                penultimateForce += Flee(friend);
            }
        }

        return penultimateForce;
    }


    /// <summary>
    /// Wander method, activates when there is nothing to evade or pursue.
    /// </summary>
    /// <returns></returns>
    public Vector3 Wander()
    {
        Vector3 circleCenter = direction * 3;
        

        if (frameCount >=30)
        {
            
            float angle = Random.Range(0f, 360f);
            float x = circleCenter.x + Mathf.Cos(angle) * wanderRadius;
            float z = circleCenter.z + Mathf.Sin(angle) * wanderRadius;
            spot = new Vector3(x, vehiclePosition.y, z);
            frameCount = 0;
        }
        else
        {           
            frameCount++;
        }

        wanderSpot = vehiclePosition + circleCenter + spot;

        Debug.DrawLine(vehiclePosition + circleCenter, wanderSpot, Color.cyan);
        Debug.DrawLine(vehiclePosition + circleCenter, vehiclePosition, Color.red);

        return Seek(wanderSpot);
    }
}
