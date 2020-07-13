using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    public float speed = 8f;
    private Rigidbody bulletRigidbody;

    void Start()
    {
        // 게임 오브젝트에서 Rigidbody Component를 찾아 bulletRigidbody에 할당
        bulletRigidbody = GetComponent<Rigidbody>();
        // Rigidbody의 속도 = 앞쪽 방향 * 이동 속력
        bulletRigidbody.velocity = transform.forward * speed;

        //3초 뒤에 자신의 gameObject 파괴
        Destroy(gameObject, 3f);
    }

    void Update()
    {
        
    }

    //Trigger 충돌 시 자동으로 실행되는 Method
    private void OnTriggerEnter(Collider other)
    {
        //충돌한 상대방 GameObject가 Player 태그를 가진 경우
        if(other.tag == "Player")
        {
            //상대방 GameObject에서 PlayerController Component 가져오기
            PlayerController playerController = other.GetComponent<PlayerController>();

            //상대방으로부터 PlayerController Component를 가져오는 데 성공했다면
            if(playerController != null)
            {
                //상대방 PlayerController 컴포넌트의 Die() Method 실행
                playerController.Die();
            }
        }
    }
}
