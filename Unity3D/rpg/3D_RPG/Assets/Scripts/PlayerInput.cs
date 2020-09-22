using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerInput : MonoBehaviour
{
    CharacterController characterController;
    PlayerControl cm;
    Animator playerAnimator;
    public GameObject escMenu;
    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();
        cm = GetComponent<PlayerControl>();
        playerAnimator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.LeftShift) || Input.GetKey(KeyCode.LeftShift))
        {
            playerAnimator.SetBool("Run", true);
        }
        if (Input.GetKeyUp(KeyCode.LeftShift))
        {
            playerAnimator.SetBool("Run", false);
        }

        if(characterController.isGrounded && Input.GetKeyDown(KeyCode.Space))
        {
            if (!playerAnimator.GetBool("Damaged") && !playerAnimator.GetBool("Jump") && !playerAnimator.GetBool("Attack"))
                cm.Jump();
        }

        if (Input.GetMouseButtonDown(0))
        {
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            Vector3 mousePoint = new Vector3();
            RaycastHit hit;
            if (Physics.Raycast(ray, out hit, 10000f, 1 << LayerMask.NameToLayer("Ground")))
            {
                mousePoint = new Vector3(hit.point.x, transform.position.y, hit.point.z);
                cm.SetDestination(mousePoint);
            }
            if (Physics.Raycast(ray, out hit, 10000f, 1 << LayerMask.NameToLayer("Enemy")))
            {
                mousePoint = new Vector3(hit.point.x, transform.position.y, hit.point.z);
                cm.SetTarget(hit.collider.gameObject);
            }
        }
        if (Input.GetMouseButtonDown(1))
        {
            cm.Skill();
        }
        if (Input.GetMouseButtonUp(1))
        {
            cm.UnBlock();
        }
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            cm.curSkillNum = 0;
        }
        if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            cm.curSkillNum = 1;
        }
        if (Input.GetKeyDown(KeyCode.Alpha3))
        {
            cm.curSkillNum = 2;
        }
        if (Input.GetKeyDown(KeyCode.Alpha4))
        {
            cm.curSkillNum = 3;
        }
        if (Input.GetKeyDown(KeyCode.Alpha5))
        {
            cm.curSkillNum = 4;
        }
        if (Input.GetKeyDown(KeyCode.I)){
            InventorySystem.instance.InventoryOpenClose();
        }
        if (Input.GetKeyDown(KeyCode.C))
        {
            CharacterInfoSystem.instance.CheacterInfoOpenClose();
        }
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if(escMenu.activeSelf == false)
            {
                escMenu.SetActive(true);
                Time.timeScale = 0.0f;
            }
            else
            {
                escMenu.SetActive(false);
                Time.timeScale = 1.0f;
            }
        }
    }
}
