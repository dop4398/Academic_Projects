using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Manages all of the ship's vectors for movement.
/// Updates position and velocity every frame.
/// Draws itself to the scene.
/// Manages the Shine ability.
/// Manages player health (the ship's hit points).
/// </summary>
public class Ship : MonoBehaviour
{
    // Fields    
    public float accelRate;
    public Vector3 position;
    public Vector3 direction;
    public Vector3 velocity;
    public Vector3 acceleration;
    public float angleOfRotation;
    public float maxSpeed;
    public float deceleration;

    private int shineTimer;
    public GameObject shine;
    public Sprite shipNormal;
    public Sprite shipShine;


    public int hitPoints;
    public int points;
    public bool justDamaged;
    public int damageTimer;
    public GameObject grunt;

    public Camera cameraObject;
    private float totalCameraHeight;
    private float totalCameraWidth;

	
    /// <summary>
    /// Initializes the ships position, direction, and velocity.
    /// </summary>
	void Start ()
    {
        position = new Vector3(0, 0, 0);
        direction = new Vector3(1, 0, 0);
        velocity = new Vector3(0, 0, 0);

        totalCameraHeight = cameraObject.orthographicSize * 2f;
        totalCameraWidth = totalCameraHeight * cameraObject.aspect;

        hitPoints = 3;
        shineTimer = 0;
        justDamaged = false;
        damageTimer = 120;
	}
	
	
    /// <summary>
    /// Updates the ships position, rotation, and velocity every frame.
    /// Checks for the Shine ability every frame.
    /// Checks for damage every frame.
    /// </summary>
	void Update ()
    {
        RotateShip();

        Drive();

        WrapShip();

        SetTransform();

        Shine();

        Damaged();
    }


    /// <summary>
    /// Changes / Sets the transform component
    /// </summary>
    void SetTransform()
    {
        // Rotate Ship sprite
        transform.rotation = Quaternion.Euler(0, 0, angleOfRotation);

        // Set the transform position
        transform.position = position;
    }


    /// <summary>
    /// Accelerates the ship while the up arrow key is pressed.
    /// </summary>
    void Drive()
    {
        acceleration = accelRate * direction;

        // Only accelerate if the up arrow key is pressed. Otherwise, decelerate.
        if (Input.GetKey(KeyCode.UpArrow))
        {
            velocity += acceleration;
        }
        else
        {
            if (velocity.sqrMagnitude > .00001f)
                velocity *= deceleration;
            else
                velocity = Vector3.zero;
        }
        

        // Limit velocity so it doesn't become too large
        velocity = Vector3.ClampMagnitude(velocity, maxSpeed);

        // Add velocity to Ship's position
        position += velocity;
    }


    /// <summary>
    /// Player can control direction
    /// Left arrow key = rotate left by 2 degrees
    /// Right arrow key = rotate right by 2 degrees
    /// </summary>
    void RotateShip()
    {
        
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            angleOfRotation += 2;
            direction = Quaternion.Euler(0, 0, 2) * direction;
        }
        else if (Input.GetKey(KeyCode.RightArrow))
        {
            angleOfRotation -= 2;
            direction = Quaternion.Euler(0, 0, -2) * direction;
        }
    }


    /// <summary>
    /// Wraps the Ship so it is always on-screen
    /// </summary>
    void WrapShip()
    {
        // X value
        if (position.x >= totalCameraWidth / 2)
        {
            position.x = -(totalCameraWidth / 2) + 0.5f;
        }
        else if (position.x <= -(totalCameraWidth / 2))
        {
            position.x = totalCameraWidth / 2 - 0.5f;
        }

        // Y value
        if (position.y >= totalCameraHeight / 2)
        {
            position.y = -(totalCameraHeight / 2) + 0.5f;
        }
        else if (position.y <= -(totalCameraHeight / 2))
        {
            position.y = totalCameraHeight / 2 - 0.5f;
        }
    }


    /// <summary>
    /// Changes the ship's sprite when left shift is pressed.
    /// ~1 second duration, ~2 second cooldown.
    /// </summary>
    void Shine()
    {
        if (Input.GetKey(KeyCode.LeftShift) && shineTimer <= 0)
        {
            GetComponent<SpriteRenderer>().sprite = shipShine;
            shineTimer = 180;  // 60 frame duration

            Instantiate(shine, transform);
        }
        else if (shineTimer > 120)  // 120 frame cooldown
        {
            GetComponent<SpriteRenderer>().sprite = shipShine;
            shineTimer--;
        }
        else
        {
            GetComponent<SpriteRenderer>().sprite = shipNormal;
            shineTimer--;
        }
    }


    /// <summary>
    /// Checks if the ship was just damaged.
    /// 120 frame invunerability window after getting damaged.
    /// </summary>
    void Damaged()
    {
        if(justDamaged && damageTimer <= 0)
        {
            damageTimer = 120;
            hitPoints--;
            justDamaged = false;
            Instantiate(grunt, transform);
        }

        justDamaged = false;

        // Have the sprite flicker between red and white while invunerable.
        if (damageTimer > 90 || (damageTimer < 85 && damageTimer > 60) || (damageTimer < 55 && damageTimer > 35) 
            || (damageTimer < 30 && damageTimer > 15) || (damageTimer < 10 && damageTimer > 0))
        {
            GetComponent<SpriteRenderer>().color = Color.red;             
        }
        else
        {
            GetComponent<SpriteRenderer>().color = Color.white;
        }

        damageTimer--;
    }
}
