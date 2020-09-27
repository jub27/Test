using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControl : MonoBehaviour
{
    private Transform target;
    private PlayerControl cm;
    private float currentDistance = 15.0f;
    private float wheelDistance = 5.0f;

    private float x = 0.0f;
    private float y = -50.0f;
    // Start is called before the first frame update
    void Start()
    {
        target = GameObject.Find("Player").GetComponent<Transform>();
        cm = GameObject.Find("Player").GetComponent<PlayerControl>();
    }

    // Update is called once per frame
    void Update()
    {
        float wheelInput = Input.GetAxis("Mouse ScrollWheel");
        currentDistance = Mathf.Clamp(currentDistance - wheelDistance*wheelInput, 5.0f, 15.0f);
        if (Input.GetMouseButton(2))
        {
            x += Input.GetAxis("Mouse X") * 4.0f;
            y -= Input.GetAxis("Mouse Y") * 3.2f;
        }
        y = Mathf.Clamp(y, -80, 20);

        Quaternion rotation = Quaternion.Euler(y, x, 0);

        transform.position = target.transform.position + rotation * Vector3.forward * currentDistance;

        transform.forward = (target.transform.position - transform.position).normalized;
    }
}