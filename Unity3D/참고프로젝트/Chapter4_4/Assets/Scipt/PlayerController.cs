#define NEW
//#define OLD

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{
    private Rigidbody playerRigidbody;
    public float speed = 8f;

    void Start()
    {
        //GameObject에서 Rigidbody 컴포넌트를 찾아 playerRigidbody에 할당
        playerRigidbody = GetComponent<Rigidbody>();
    }

    void Update()
    {
#if NEW
        //수평축과 수직축의 입력값을 감지하여 저장
        float xInput = Input.GetAxis("Horizontal");
        float zInput = Input.GetAxis("Vertical");

        //실제 이동속도를 입력갓과 이동 속력을 사용해 결정
        float xSpeed = xInput * speed;
        float zSpeed = zInput * speed;

        //Vector3 속도를 (xSpeed, 0, zSpeed)로 생성
        Vector3 moveVelocity = new Vector3(xSpeed, 0f, zSpeed);
        //리지드바디의 속도에 moveVelocity를 할당
        playerRigidbody.velocity = moveVelocity;

#elif OLD
        //개선전 사항
        if(Input.GetKey(KeyCode.UpArrow))
        {
            //위쪽 방향키 입력이 감지된 경우 z 방향 힘 주기
            playerRigidbody.AddForce(0f, 0f, speed);
        }

        if (Input.GetKey(KeyCode.DownArrow))
        {
            //아래쪽 방향키 입력이 감지된 경우 -z 방향 힘 주기
            playerRigidbody.AddForce(0f, 0f, -speed);
        }

        if (Input.GetKey(KeyCode.RightArrow))
        {
            //오른쪽 방향키 입력이 감지된 경우 x 방향 힘 주기
            playerRigidbody.AddForce(speed, 0f, 0f);
        }

        if (Input.GetKey(KeyCode.LeftArrow))
        {
            //아래쪽 방향키 입력이 감지된 경우 -x 방향 힘 주기
            playerRigidbody.AddForce(-speed, 0f, 0f);
        }

        if(Input.GetKey(KeyCode.W))
        {
            //Object의 정면으로 진행하는 방향으로 1크기 만큼 이동변환을 한다.
            transform.Translate(transform.forward, Space.World);
        }

        if (Input.GetKey(KeyCode.S))
        {
            //Object의 정면으로 진행하는 방향으로 -1크기 만큼 이동변환을 한다.
            transform.Translate(-transform.forward, Space.World);
        }

        if (Input.GetKey(KeyCode.A))
        {
            //Object의 오른쪽으로 진행하는 방향으로 -1크기 만큼 이동변환을 한다.
            transform.Translate(-transform.right, Space.World);
        }

        if (Input.GetKey(KeyCode.D))
        {
            //Object의 오른쪽으로 진행하는 방향으로 1크기 만큼 이동변환을 한다.
            transform.Translate(transform.right, Space.World);
        }

        if (Input.GetKey(KeyCode.Q))
        {
            //Object의 Up Vector(y축) 방향을 축으로 반시계 방향으로 회전 변환한다.
            transform.Rotate(Vector3.up, -5, Space.World);
        }

        if (Input.GetKey(KeyCode.E))
        {
            //Object의 Up Vector(y축) 방향을 축으로 시계 방향으로 회전 변환한다.
            transform.Rotate(Vector3.up, 5, Space.World);
        }
#endif
    }

    public void Die()
    {
        //자신의 GameObject를 비활성화
        gameObject.SetActive(false);

        //Scene에 존재하는 GameManager 타입의 오브젝트를 찾아서 가져오기
        GameManager gameManager = FindObjectOfType<GameManager>();

        //가져온 GameManager 오브젝트의 EndGame() Method 실행
        gameManager.EndGame();
    }
}
