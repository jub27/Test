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
        // 마우스 휠로 카메라와 캐릭터 사이의 기본 거리를 설정
        distance = Mathf.Clamp(distance - wheelDistance * wheelInput, 5.0f, 15.0f);
        // 카메라와 캐릭터 사이에 장애물이 있는 경우 카메라를 옮겨야 할 위치를 설정
        float targetDistance = AdjustLineOfSight(target.position, (transform.position - target.position).normalized);
        // 최종적으로 카메라와 캐릭터 사이의 거리 설정
        currentDistance = Mathf.SmoothDamp(currentDistance, targetDistance, ref distanceVelocity, 0.3f);
        if (Input.GetMouseButton(2))// 마우스 움직임 값 가져오기
        {
            x += Input.GetAxis("Mouse X") * 4.0f;
            y -= Input.GetAxis("Mouse Y") * 3.2f;
        }
        y = Mathf.Clamp(y, -80, 20);
        // 카메라 각도 값 설정
        Quaternion rotation = Quaternion.Euler(y, x, 0);
        //플레이어 캐릭터로부터 위에서 계산한 거리와 각도를 이용하여 카메라 위치 설정
        transform.position = target.transform.position + rotation * Vector3.forward * currentDistance;
        transform.forward = (target.transform.position - transform.position).normalized;
    }
    // 캐릭터와 카메라 사이에 장애물이 있는경우 (캐릭터와 장애물 사이의 거리 -0.2f) 를 반환
    private float AdjustLineOfSight(Vector3 target, Vector3 direction)
    {
        RaycastHit hit;
        if (Physics.Raycast(target, direction, out hit, distance, 1 << LayerMask.NameToLayer("Ground")))
        {
            if (hit.distance - 0.2f < 2.0f) //장애물과 캐릭터가 아주 가까운 경우는 카메라를 옮기지 않음
                return distance;
            else
                return hit.distance - 0.2f;
        }
        else
            return distance;
    }
}