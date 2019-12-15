using System.Collections;
using System.Collections.Generic;
using UnityEngine;


/// <summary>
/// Handles the graphical user interface.
/// displays lives left and player score.
/// Determins and resolves the loss condition.
/// </summary>
public class GUIManager : MonoBehaviour
{
    public GameObject ship;
    public Camera cameraObject;
    public GameObject death;

    
    void Start()
    {
        
    }

    
    /// <summary>
    /// Checks for the loss condition (losing all hitpoints) every frame.
    /// Quits the application upon reaching that condition.
    /// </summary>
    void Update()
    {
        if (ship.GetComponent<Ship>().hitPoints <= 0 || Input.GetKeyDown(KeyCode.Escape))
        {
            Instantiate(death, transform);
            Destroy(ship);
            Invoke("EndGame", 3f);
        }
    }


    /// <summary>
    /// Displays the player's hitpoints and score on the screen.
    /// </summary>
    void OnGUI()
    {
        GUI.color = Color.white;
        GUI.skin.box.fontSize = 18;

        GUI.Box(new Rect(10, 10, 250, 55), "Ship health: " + ship.GetComponent<Ship>().hitPoints +
            "\nPoints: " + ship.GetComponent<Ship>().points);
    }


    /// <summary>
    /// Quits the application when called.
    /// </summary>
    void EndGame()
    {
        Application.Quit();
    }
}
