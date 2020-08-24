using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraControl : MonoBehaviour
{
    public Transform target;
    private CharacterMove cm;
    private LayerMask layerMask = 8;
    private Vector3 targetOffset = new Vector3(0.0f, 1.5f, 0.0f);
    private float currentDistance = 10.0f;
    private float distance = 10.0f;

    private float x = 0.0f;
    private float y = 0.0f;

    private float distanceVelocity = 0.0f;

    private Vector3 positionSave;
    private Quaternion rotationSave;
    // Start is called before the first frame update
    void Start()
    {
        cm = GameObject.Find("Player").GetComponent<CharacterMove>();
        Vector3 angles = transform.eulerAngles;
        x = angles.y;
        y = angles.x;
    }

    // Update is called once per frame
    void Update()
    {
        x += Input.GetAxis("Mouse X") * 4.0f;
        y -= Input.GetAxis("Mouse Y") * 3.2f;

        y = Mathf.Clamp(y, -20, 80);

        Quaternion rotation = Quaternion.Euler(y, x, 0);

        Vector3 targetPos = target.position + targetOffset;

        Vector3 direction = rotation * -Vector3.forward;

        float targetDistance = AdjustLineOfSight(targetPos, direction);
        
        currentDistance = Mathf.SmoothDamp(currentDistance, targetDistance, ref distanceVelocity, 0.06f);

        transform.rotation = rotation;

        transform.position = targetPos + direction * currentDistance;

        //캐릭터 정면을 카메라 정면으로 설정
        Vector3 playerDir = transform.forward;
        playerDir.y = 0;
        playerDir.Normalize();
        cm.SetPlayerDirection(playerDir);
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