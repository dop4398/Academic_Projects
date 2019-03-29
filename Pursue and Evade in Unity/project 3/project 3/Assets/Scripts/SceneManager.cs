using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Scene manager class.
/// Keeps track of all humans, zombies, and tree obstacles.
/// </summary>
public class SceneManager : MonoBehaviour
{
    // Fields
    public int numberOfHumans;
    public int numberOfObstacles;
    public GameObject humanPrefab;
    public GameObject zombiePrefab;
    public GameObject treePrefab1;
    public GameObject treePrefab2;
    public GameObject treePrefab3;
    public GameObject treePrefab4;
    private GameObject obstaclePrefab;
    public List<GameObject> humans;
    public List<GameObject> zombies;
    public List<GameObject> obstacles;
    public int parkWidth;
    public int parkLength;
    private bool debugLinesOn;


    void Start ()
    {
        humans = new List<GameObject>();
        zombies = new List<GameObject>();
        obstacles = new List<GameObject>();
        int x = 0;
        int z = 0;
        int chosenPrefab;
        debugLinesOn = false;


        // Instantiate the obstacles
        for (int i = 0; i < numberOfObstacles; i++)
        {
            x = Random.Range(-parkWidth + 3, parkWidth - 3);
            z = Random.Range(-parkLength + 3, parkLength - 3);
            chosenPrefab = Random.Range(0, 4);
            if(chosenPrefab == 0)
            {
                obstaclePrefab = treePrefab1;
            }
            else if(chosenPrefab == 1)
            {
                obstaclePrefab = treePrefab2;
            }
            else if (chosenPrefab == 2)
            {
                obstaclePrefab = treePrefab3;
            }
            else
            {
                obstaclePrefab = treePrefab4;
            }

            obstacles.Add(Instantiate(obstaclePrefab, new Vector3(x, 0, z), Quaternion.identity));
        }

        // Instantiate all the humans
        for (int i = 0; i < numberOfHumans; i++)
        {
            x = Random.Range(-parkWidth + 1, parkWidth - 1);
            z = Random.Range(-parkLength + 1, parkLength - 1);
            AddHuman(new Vector3(x, 0, z));
        }

        // Instantiate the zombie
        x = Random.Range(-parkWidth + 1, parkWidth - 1);
        z = Random.Range(-parkLength + 1, parkLength - 1);
        AddZombie(new Vector3(x, 0, z));
    }


    void Update ()
    {
        // Update the target location of the humans and all of the lists within every object
        UpdateObjects();

        // Update infected humans
        if(humans.Count > 0)
        {
            UpdateInfected();
        }

        // Debug Lines
        if (Input.GetKeyDown(KeyCode.D))
        {
            debugLinesOn = !debugLinesOn;
        }
    }


    /// <summary>
    /// Updates the lists of humans and zombies in every object.
    /// </summary>
    void UpdateObjects()
    {
        if(humans.Count > 0)
        {
            foreach (GameObject human in humans)
            {
                UpdateLists(human.GetComponent<Human>());
                human.GetComponent<Vehicle>().debugLinesOn = debugLinesOn;
            }
        }
        
        foreach(GameObject zombie in zombies)
        {
            UpdateLists(zombie.GetComponent<Zombie>());
            zombie.GetComponent<Vehicle>().debugLinesOn = debugLinesOn;

        }
    }


    /// <summary>
    /// Whenever a zombie collides with a human, turn that human into a zombie.
    /// </summary>
    void UpdateInfected()
    {
        int infectedIndex = -1;

        foreach (GameObject zombie in zombies)
        {
            for (int i = 0; i < humans.Count; i++)
            {
                if (Collision(zombie.GetComponent<Zombie>().vehiclePosition, humans[i].GetComponent<Human>().vehiclePosition))
                {
                    infectedIndex = i;
                    break;
                }
            }
            if(infectedIndex != -1)
            {
                break;
            }
        }

        // Add the newly infected to the zombies list,
        //  instantiate them at the infected humans' positions,
        //  and remove the infected humans from the humans list.
        Vector3 infectedPosition;
        GameObject tempHuman;

        if(humans.Count > 0 && infectedIndex != -1)
        {
            infectedPosition = humans[infectedIndex].GetComponent<Human>().vehiclePosition;
            Destroy(humans[infectedIndex].GetComponent<Vehicle>().debugSphere);

            AddZombie(infectedPosition);
            
            tempHuman = humans[infectedIndex];
            humans.RemoveAt(infectedIndex);
            Destroy(tempHuman);
        }
    }


    /// <summary>
    /// Updates the lists of a Human object.
    /// </summary>
    /// <param name="human"></param>
    void UpdateLists(Human human)
    {
        human.humans = humans;
        human.zombies = zombies;
    }

    /// <summary>
    /// Updates the lists of a Zombie object.
    /// </summary>
    /// <param name="zombie"></param>
    void UpdateLists(Zombie zombie)
    {
        zombie.humans = humans;
        zombie.zombies = zombies;
    }


    /// <summary>
    /// Adds a single human to the scene at the specified position.
    /// </summary>
    void AddHuman(Vector3 position)
    {
        
        humans.Add(Instantiate(humanPrefab, position, Quaternion.identity));
        humans[humans.Count - 1].GetComponent<Human>().closestZombie = null;
        humans[humans.Count - 1].GetComponent<Vehicle>().obstacles = obstacles;
        humans[humans.Count - 1].GetComponent<Vehicle>().debugLinesOn = false;
    }


    /// <summary>
    /// Adds a single zombie to the scene at the specified position.
    /// </summary>
    void AddZombie(Vector3 position)
    {
        zombies.Add(Instantiate(zombiePrefab, position, Quaternion.identity));
        zombies[zombies.Count - 1].GetComponent<Zombie>().closestHuman = null;
        zombies[zombies.Count - 1].GetComponent<Vehicle>().obstacles = obstacles;
        zombies[zombies.Count - 1].GetComponent<Vehicle>().debugLinesOn = false;
    }

    /// <summary>
    /// Checks if two vectors are within sqrt(2) unit of each other.
    /// </summary>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <returns>True if vectors are within sqrt(2) units of each other, false otherwise.</returns>
    bool Collision(Vector3 a, Vector3 b)
    {
        float distance = (a - b).sqrMagnitude;

        if(distance < 2.5)
        {
            return true;
        }
        return false;
    }

    void OnGUI()
    {
        GUI.color = Color.white;
        GUI.skin.box.fontSize = 16;

        if(GUI.Button(new Rect(10, 140, 150, 30), "Add a Peasant"))
        {
            int x = Random.Range(-parkWidth + 1, parkWidth - 1);
            int z = Random.Range(-parkLength + 1, parkLength - 1);
            AddHuman(new Vector3(x, 0, z));
        }

        if(GUI.Button(new Rect(10, 180, 150, 30), "Add a Goblin"))
        {
            int x = Random.Range(-parkWidth + 1, parkWidth - 1);
            int z = Random.Range(-parkLength + 1, parkLength - 1);
            AddZombie(new Vector3(x, 0, z));
        }

        if(GUI.Button(new Rect(10, 220, 150, 30), "Wipe the Board"))
        {
            foreach(GameObject human in humans)
            {
                Destroy(human.GetComponent<Vehicle>().debugSphere);
                Destroy(human);
            }
            humans.Clear();

            foreach(GameObject zombie in zombies)
            {
                Destroy(zombie.GetComponent<Vehicle>().debugSphere);
                Destroy(zombie);
            }
            zombies.Clear();
        }
    }
}