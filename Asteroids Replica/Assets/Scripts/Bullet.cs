using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Initializes all the necessary vectors of a bullet.
/// Updates and draws its position every frame.
/// Resolves screen wrapping of bullets.
/// </summary>
public class Bullet : MonoBehaviour
{
    // Fields
    public Vector3 position;
    public Vector3 direction;
    public Vector3 velocity;
    public GameObject ship;
    private float speed;
    public float cameraWidth;
    public float cameraHeight;


    /// <summary>
    /// Initializes the speed, position, direction, and velocity of the bullet.
    /// </summary>
    void Start ()
    {
        speed = 0.15f;
        position = ship.GetComponent<Ship>().position;
        direction = ship.GetComponent<Ship>().direction;
        velocity = speed * direction;
    }
	
	
    /// <summary>
    /// Updates the position of the bullet every frame.
    /// </summary>
	void Update ()
    {
        position += velocity;
        transform.position = position;
        WrapBullet();
    }


    /// <summary>
    /// Wraps the bullet if it goes off screen
    /// </summary>
    void WrapBullet()
    {
        // X value
        if (position.x >= cameraWidth)
        {
            position.x = -cameraWidth + 0.5f;
        }
        else if (position.x <= -cameraWidth)
        {
            position.x = cameraWidth - 0.5f;
        }

        // Y value
        if (position.y >= cameraHeight)
        {
            position.y = -cameraHeight + 0.5f;
        }
        else if (position.y <= -cameraHeight)
        {
            position.y = cameraHeight - 0.5f;
        }
    }
}
