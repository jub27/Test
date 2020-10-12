using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControl : MonoBehaviour
{
    private Transform target;
    private PlayerControl cm;
    private float currentDistance = 15.0f;
    private float distance = 15.0f;
    private float wheelDistance = 5.0f;
    private float wheelDestination = 15.0f;
    private LayerMask lineOfSightMask = -1;
    private float distanceVelocity = 0.0f;
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
        distance = Mathf.Clamp(distance - wheelDistance * wheelInput, 5.0f, 15.0f);
        float targetDistance = AdjustLineOfSight(target.position, (transform.position - target.position).normalized);
        currentDistance = Mathf.SmoothDamp(currentDistance, targetDistance, ref distanceVelocity, 0.3f);
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
    private float AdjustLineOfSight(Vector3 target, Vector3 direction)
    {
        RaycastHit hit;
        if (Physics.Raycast(target, direction, out hit, distance, 1 << LayerMask.NameToLayer("Ground"), QueryTriggerInteraction.UseGlobal))
        {
            if (hit.distance - 0.2f > 2.0f)
                return hit.distance - 0.2f;
            else
                return distance;
        }
        else
            return distance;
    }
}