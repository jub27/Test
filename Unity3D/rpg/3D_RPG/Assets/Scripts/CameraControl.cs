using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControl : MonoBehaviour
{
    private Transform target;
    private PlayerControl cm;
    private LayerMask layerMask = 8;
    private float currentDistance = 15.0f;
    private float wheelDistance = 5.0f;
    private float distance = 10.0f;

    private float x = 0.0f;
    private float y = -50.0f;

    private float distanceVelocity = 0.0f;

    private Vector3 positionSave;
    private Quaternion rotationSave;
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

        /*
        Vector3 targetPos = target.position + targetOffset;

        Vector3 direction = rotation * -Vector3.forward;

        float targetDistance = AdjustLineOfSight(targetPos, direction);
        
        currentDistance = Mathf.SmoothDamp(currentDistance, targetDistance, ref distanceVelocity, 0.06f);

        transform.rotation = rotation;

        transform.position = targetPos + direction * currentDistance;
        /*
        //캐릭터 정면을 카메라 정면으로 설정
        Vector3 playerDir = transform.forward;
        playerDir.y = 0;
        playerDir.Normalize();
        cm.SetPlayerDirection(playerDir);
        */
    }

    private float AdjustLineOfSight(Vector3 target, Vector3 direction)
    {
        RaycastHit hit;

        if (Physics.Raycast(target, direction, out hit, distance, 1 << LayerMask.NameToLayer("Ground")))
            return hit.distance - 0.2f; // Closer Radius
        else
            return distance;
    }
}