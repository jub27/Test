using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerInput : MonoBehaviour
{
    CharacterController characterController;
    CharacterMove cm;
    Animator playerAnimator;

    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();
        cm = GetComponent<CharacterMove>();
        playerAnimator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        float v = Input.GetAxis("Vertical");
        float h = Input.GetAxis("Horizontal");

        if(Input.GetKeyDown(KeyCode.LeftShift) || Input.GetKey(KeyCode.LeftShift))
        {
            cm.isRunning = true;
        }
        if (Input.GetKeyUp(KeyCode.LeftShift))
        {
            cm.isRunning = false;
        }

        if(h != 0 || v != 0)
        {
            Vector3 vVec = Camera.main.transform.forward;
            vVec.y = 0;
            vVec.Normalize();
            vVec *= v;
            Vector3 hVec = Camera.main.transform.right;
            hVec.y = 0;
            hVec.Normalize();
            hVec *= h;
            cm.Move(vVec + hVec);
        }
        if(characterController.isGrounded && Input.GetKeyDown(KeyCode.Space))
        {
            cm.Jump();
        }

        if (Input.GetMouseButtonDown(0))
        {
            playerAnimator.SetTrigger("Attack");
        }

        if (Input.GetMouseButtonDown(1))
        {
            cm.Block();
        }
        if (Input.GetMouseButtonUp(1))
        {
            cm.UnBlock();
        }
    }
}
