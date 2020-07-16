using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BulletScript : MonoBehaviour
{
    public float speed = 8f;
    private Rigidbody bulletRigidbody;

    private void OnTriggerEnter(Collider other)
    {
        if(other.tag == "player")
        {
            PlayerController playerController = other.GetComponent<PlayerController>();
            
            if(playerController != null)
            {
                playerController.Die();
            }
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        bulletRigidbody = GetComponent<Rigidbody>();
        bulletRigidbody.velocity = transform.forward * speed;
        Destroy(gameObject, 3f);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
