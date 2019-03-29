using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Manages collisions between the ship, its bullets, and asteroids every frame.
/// Insantiates the asteroids at the start of the game.
/// Creates level 2 asteroids when level 1 asteroids are destroyed.
/// Determines when the victory condition (all asteroids are destroyed) is met.
/// </summary>
public class AsteroidsManager1 : MonoBehaviour
{
    // Fields
    public GameObject ship;
    public Sprite shipShine;
    public GameObject bigPrefab;
    public GameObject smallPrefab;
    private List<GameObject> asteroids;
    public int numberOfAsteroids;
    public Camera cameraObject;
    public GameObject bulletManager;
    public GameObject victory;
    public GameObject targetHit;
    public bool missionComplete;

    // Variables for level 2 astroid generation
    private Vector3 tempPosition;
    private Vector3 tempDirection;
    private GameObject tempAsteroid1;
    private GameObject tempAsteroid2;
    private bool asteroidHit;
    private int indexA;
    private int indexB;
    private int levelOfDestroyedAsteroid;


    /// <summary>
    /// Instantiates all of the level 1 asteroids at the start of the game.
    /// </summary>
    void Start ()
    {
        asteroids = new List<GameObject>();
        asteroidHit = false;
        indexA = 0;
        indexB = 0;
        missionComplete = false;

        // Instantiate the asteroids
        for (int x = 0; x < numberOfAsteroids; x++)
        {
            asteroids.Add(Instantiate(bigPrefab, bigPrefab.GetComponent<Asteroid1>().position, Quaternion.identity));
            asteroids[x].GetComponent<Asteroid1>().cameraHeight = cameraObject.orthographicSize;
            asteroids[x].GetComponent<Asteroid1>().cameraWidth = cameraObject.orthographicSize * cameraObject.aspect;
            asteroids[x].GetComponent<Asteroid1>().level = 1;
            asteroids[x].GetComponent<Asteroid1>().ship = ship;
        }
    }
	
	
    /// <summary>
    ///  Checks all collisions and the win condition every frame.
    /// </summary>
	void Update ()
    {
        CheckAllCollisions();

        CheckVictory();
    }


    /// <summary>
    /// Determines if two objects are colliding.
    /// Utilizes circle hitboxes.
    /// </summary>
    /// <param name="obj1"></param>
    /// <param name="obj2"></param>
    /// <returns></returns>
    bool CircleCollision(GameObject obj1, GameObject obj2)
    {
        // Set the radii equal to (90% of) half the width (to reduce false positives)
        float obj1Rad = obj1.GetComponent<SpriteRenderer>().bounds.extents.x * 0.9f;
        float obj2Rad = obj2.GetComponent<SpriteRenderer>().bounds.extents.x * 0.9f;

        // Get the distance from once center to the other
        Vector3 distance = obj1.GetComponent<SpriteRenderer>().bounds.center -
                        obj2.GetComponent<SpriteRenderer>().bounds.center;

        // Check if its magnitude squared is less than the sum of the redii squared
        if (distance.sqrMagnitude < (obj1Rad + obj2Rad) * (obj1Rad + obj2Rad))
        {
            return true;
        }
        return false;
    }


    /// <summary>
    /// Checks collisions between the ship, its bullets, and asteroids each frame.
    /// Determines when asteroids are deflected by the ship.
    /// Creates new second level asteroids when first level asteroids are destroyed.
    /// </summary>
    void CheckAllCollisions()
    {
        asteroidHit = false;
        levelOfDestroyedAsteroid = 2;

        // For loop checking asteroid and ship collisions
        foreach (GameObject asteroid in asteroids)
        {
            if (CircleCollision(ship, asteroid))
            {
                // If the ship is using its Shine move, deflect the asteroid.
                if(ship.GetComponent<SpriteRenderer>().sprite == shipShine)
                {
                    asteroid.GetComponent<Asteroid1>().deflected = true;
                }
                else // Otherwise, take damage as usual.
                {
                    ship.GetComponent<Ship>().justDamaged = true;
                }           
            }

            // For loop checking each asteroid against each bullet
            foreach (GameObject bullet in bulletManager.GetComponent<BulletManager>().bullets)
            {
                if (bullet != null && CircleCollision(bullet, asteroid))
                {
                    // If its a level 1 asteroid being hit, create a new level 2 asteroid.
                    if(asteroid.GetComponent<Asteroid1>().level == 1)
                    {
                        // Make the first small asteroid
                        tempDirection = asteroid.GetComponent<Asteroid1>().direction;
                        tempPosition = new Vector3(asteroid.GetComponent<Asteroid1>().position.x - tempDirection.y,
                                                    asteroid.GetComponent<Asteroid1>().position.y + tempDirection.x, 0);
                        tempDirection = Quaternion.Euler(0, 0, 20) * tempDirection;

                        tempAsteroid1 = Instantiate(smallPrefab, tempPosition, Quaternion.identity);
                        tempAsteroid1.GetComponent<Asteroid1>().Level2Setup(tempPosition, tempDirection);

                        // Make the second small asteroid
                        tempDirection = asteroid.GetComponent<Asteroid1>().direction;
                        tempPosition = new Vector3(asteroid.GetComponent<Asteroid1>().position.x + tempDirection.y,
                                                    asteroid.GetComponent<Asteroid1>().position.y - tempDirection.x, 0);
                        tempDirection = Quaternion.Euler(0, 0, 340) * tempDirection;

                        tempAsteroid2 = Instantiate(smallPrefab, tempPosition, Quaternion.identity);
                        tempAsteroid2.GetComponent<Asteroid1>().Level2Setup(tempPosition, tempDirection);

                        levelOfDestroyedAsteroid = 1;
                    }
                    

                    // Destroy the big asteroid, get its index in the list to remove it after the loop
                    indexA = asteroids.IndexOf(asteroid);
                    Destroy(asteroid);

                    // Destroy the bullet, get its index in the list to remove it after the loop
                    indexB = bulletManager.GetComponent<BulletManager>().bullets.IndexOf(bullet);
                    Destroy(bullet);

                    asteroidHit = true;
                }          
            }
        }

        if(asteroidHit)
        {
            // remove the destroyed things from their lists
            asteroids.RemoveAt(indexA);
            bulletManager.GetComponent<BulletManager>().bullets.RemoveAt(indexB);    
            
            if(levelOfDestroyedAsteroid == 1)
            {
                asteroids.Add(tempAsteroid1);
                asteroids[asteroids.Count - 1].GetComponent<Asteroid1>().cameraHeight = cameraObject.orthographicSize;
                asteroids[asteroids.Count - 1].GetComponent<Asteroid1>().cameraWidth = cameraObject.orthographicSize * cameraObject.aspect;
                asteroids[asteroids.Count - 1].GetComponent<Asteroid1>().ship = ship;

                asteroids.Add(tempAsteroid2);
                asteroids[asteroids.Count - 1].GetComponent<Asteroid1>().cameraHeight = cameraObject.orthographicSize;
                asteroids[asteroids.Count - 1].GetComponent<Asteroid1>().cameraWidth = cameraObject.orthographicSize * cameraObject.aspect;
                asteroids[asteroids.Count - 1].GetComponent<Asteroid1>().ship = ship;

                ship.GetComponent<Ship>().points += 20;
            }
            else
            {
                ship.GetComponent<Ship>().points += 50;
            }

            // Sound effect on hit
            Instantiate(targetHit, transform);
        }
    }


    /// <summary>
    /// The player wins once all of the asteroids are destroyed.
    /// </summary>
    void CheckVictory()
    {
        if (asteroids.Count == 0 && !missionComplete)
        {
            Instantiate(victory, transform);
            missionComplete = true;
            Invoke("EndGame", 3f);
        }
    }

    /// <summary>
    /// Quits the application when called.
    /// </summary>
    void EndGame()
    {
        Application.Quit();
    }
}
