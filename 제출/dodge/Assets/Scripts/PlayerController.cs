using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerController : MonoBehaviour
{

    public Rigidbody playerRigidbody;
    public float speed = 8f;

    public void Die()
    {
        gameObject.SetActive(false);
    }

    private void Awake()
    {
        playerRigidbody = GetComponent<Rigidbody>();
    }


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float xInput = Input.GetAxis("Horizontal");
        float zInput = Input.GetAxis("Vertical");

        float xSpeed = xInput * speed;
        float zSpeed = zInput * speed;

        Vector3 moveVelocity = new Vector3(xSpeed, 0, zSpeed);

        playerRigidbody.velocity = moveVelocity;
    }
}
