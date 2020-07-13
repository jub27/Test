using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LogTest : MonoBehaviour
{
    void Start()
    {
        Debug.Log("Log Test.... Normal Log");
        Debug.LogWarning("Log Test.... Warning Log");
        Debug.LogError("Log Test.... Error Log");
    }

    void Update()
    {
        
    }
}
