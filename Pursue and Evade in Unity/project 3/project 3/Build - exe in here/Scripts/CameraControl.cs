using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Camera controller class.
/// Keeps track of all the cameras in the scene and lets the user shift between them.
/// </summary>
public class CameraControl : MonoBehaviour {

    // Camera array that holds a reference to every camera in the scene
    public Camera[] cameras;

    // Current camera
    private int currentCameraIndex;

    void Start()
    {
        currentCameraIndex = 0;

        // Turn all cameras off except the first default one
        for (int i = 1; i < cameras.Length; i++)
        {
            cameras[i].gameObject.SetActive(false);
        }

        // If any cameras were added to the controller, enable the first one
        if (cameras.Length > 0)
        {
            cameras[0].gameObject.SetActive(true);
        }
    }

    void Update()
    {
        // Press the 'C' key to cycle through cameras in the array
        if (Input.GetKeyDown(KeyCode.C))
        {
            // Cycle to the next camera
            currentCameraIndex++;

            // If cameraIndex is in bounds, set this camera active and the last one inactive
            if (currentCameraIndex < cameras.Length)
            {
                cameras[currentCameraIndex - 1].gameObject.SetActive(false);
                cameras[currentCameraIndex].gameObject.SetActive(true);
            }
            // If last camera, cycle back to first camera
            else
            {
                cameras[currentCameraIndex - 1].gameObject.SetActive(false);
                currentCameraIndex = 0;
                cameras[currentCameraIndex].gameObject.SetActive(true);
            }
        }
    }


    void OnGUI()
    {
        GUI.color = Color.white;
        GUI.skin.box.fontSize = 16;

        GUI.Box(new Rect(10, 80, 200, 45), "Press the 'D' key to enable debug lines.");

        // Changes the GUI box for each camera view
        if (currentCameraIndex == 0)
        {
            GUI.Box(new Rect(10, 10, 275, 65), "Press the 'C' key to change the view.\nCamera 1\nOverview");
        }
        else if (currentCameraIndex == 1)
        {
            GUI.Box(new Rect(10, 10, 275, 65), "Press the 'C' key to change the view.\nCamera 2\nSide View");
        }
        else if (currentCameraIndex == 2)
        {
            GUI.Box(new Rect(10, 10, 275, 65), "Press the 'C' key to change the view.\nCamera 3\nBack View");
        }
        else
        {
            GUI.Box(new Rect(10, 10, 275, 65), "Press the 'C' key to change the view.\nCamera 4\nWide Shot");
        }
        GUI.skin.box.wordWrap = true;
    }
}
