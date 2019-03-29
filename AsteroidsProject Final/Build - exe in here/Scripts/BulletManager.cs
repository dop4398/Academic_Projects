using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Instantiates bullets when conditions are met.
/// Keeps track of all the bullets on the screen.
/// Controls bullet timing.
/// </summary>
public class BulletManager : MonoBehaviour
{
    // Fields
    public GameObject ship;
    public GameObject prefab;
    public List<GameObject> bullets;
    public Camera cameraObject;
    public int shotInterval;
    private int shotTimer;
    private List<int> deadBulletIndices;


    /// <summary>
    /// Sets up the bullet prefab to make clones of.
    /// Determines the ship's rate of fire.
    /// </summary>
	void Start ()
    {
        prefab.GetComponent<Bullet>().ship = ship;
        bullets = new List<GameObject>();
        shotInterval = 20;
        shotTimer = 30; // arbitrary value, just needs to be >= shotInterval
        deadBulletIndices = new List<int>();
	}
	
	
    /// <summary>
    /// Checks for input every frame,
    ///  uses that input to determine when to insantiate a new bullet.
    /// Destroys bullets and removes them from the list of bullets after a certain interval.
    /// </summary>
	void Update ()
    {
        // Create a new bullet when space is pressed. Int at the end determines speed of shots.
        if (Input.GetKeyDown(KeyCode.Space) && shotTimer >= shotInterval)
        {
            // Reset the timer
            shotTimer = 0;

            // Create a bullet
            bullets.Add(Instantiate(prefab, ship.GetComponent<Ship>().position, Quaternion.identity));
            bullets[bullets.Count - 1].GetComponent<Bullet>().cameraHeight = cameraObject.orthographicSize;
            bullets[bullets.Count - 1].GetComponent<Bullet>().cameraWidth = cameraObject.orthographicSize * cameraObject.aspect;

            Destroy(bullets[bullets.Count - 1], 2f);  // Currently chosen interval -> 2 seconds.
        }

        shotTimer++;

        // Remove bullets with null values from the list. (the ones that have been destroyed)
        deadBulletIndices.Clear();

        foreach (GameObject bullet in bullets)
        {
            if (bullet == null)
            {
                deadBulletIndices.Add(bullets.IndexOf(bullet));
            }
        }
        
        foreach(int index in deadBulletIndices)
        {
            bullets.RemoveAt(index);
        }
    }
}
