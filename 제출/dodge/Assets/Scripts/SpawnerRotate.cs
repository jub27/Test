﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnerRotate : MonoBehaviour
{
    private float rotationSpeed = 8f;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        transform.Rotate(0f, rotationSpeed * Time.deltaTime, 0f);
    }
}
