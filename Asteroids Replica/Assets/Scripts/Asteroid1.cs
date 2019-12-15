using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Script for asteroid prefabs.
/// Updates its position on every frame.
/// Draws itself to the scene.
/// Resolves "deflections" when the ship uses Shine.
/// Resolves screen wrapping of asteroids.
/// </summary>
public class Asteroid1 : MonoBehaviour
{
    // Fields
    public List<Sprite> asteroidSprites;
    public GameObject ship;
    public Vector3 position;
    public Vector3 direction;
    public Vector3 velocity;
    public int level;
    private float speed;
    public float cameraWidth;
    public float cameraHeight;
    private float angleOfRotation;
    public bool deflected;
    private int deflectTimer;


    /// <summary>
    /// Initializes position, direction, and velocity vectors.
    /// Sets the speed of the asteroid.
    /// Randomly chooses the sprite from a list of sprites.
    /// </summary>
    void Start ()
    {
        speed = 0.025f;

        // If it is a level 1 asteroid, randomly set the position and direction.
        if (level != 2)
        {
            float x = Random.Range(-cameraWidth, cameraWidth);
            float y = Random.Range(-cameraHeight, cameraHeight);
            position = new Vector3(x, y, 0);

            direction = new Vector3(1, 0, 0);
            angleOfRotation = Random.Range(0, 360);
            direction = Quaternion.Euler(0, 0, angleOfRotation) * direction;
            direction.Normalize();

            level = 1;
        }
        else
        {
            speed *= 1.5f;
        }

        GetComponent<SpriteRenderer>().sprite = asteroidSprites[Random.Range(0, asteroidSprites.Count)];

        velocity = speed * direction;
        deflected = false;
        deflectTimer = 0;
    }
	
	
    /// <summary>
    /// Updates and draws the asteroid every frame.
    /// </summary>
	void Update ()
    {
        Deflect();

        velocity = speed * direction;
        position += velocity;
        transform.position = position;

        WrapAsteroid();
    }


    /// <summary>
    /// Wraps the asteroid if it goes off screen
    /// </summary>
    public void WrapAsteroid()
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


    /// <summary>
    /// Method to set up the level 2 asteroids.
    /// TO DO: implement a slight offset in position and direction.
    ///        Do that either here or before the method is called in the AstroidManager class.
    /// </summary>
    /// <param name="pos"></param>
    /// <param name="dir"></param>
    public void Level2Setup(Vector3 pos, Vector3 dir)
    {
        position = pos;
        direction = dir;
        direction.Normalize();
        level = 2;
        velocity = speed * direction;
    }


    /// <summary>
    /// If the ship's Shine ability is up, then the asteroid gets deflected.
    /// The timer is used to prevent the direction change from happening every frame.
    /// </summary>
    void Deflect()
    {
        if(deflected && deflectTimer <= 0)
        {
            direction = new Vector3(-direction.y, direction.x, 0);
            deflectTimer = 120;
        }

        deflected = false;
        deflectTimer--;
    }
}