using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RemoveBullet : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    private void OnCollisionEnter(Collision coll)
    {
        Debug.Log("adsd");
        //충돌한 게임오브젝트의 태그값 비교
        if (coll.gameObject.tag == "bullet")
        {
            //충돌한 게임오브젝트 삭제
            Destroy(coll.gameObject);
        }
    }
}
