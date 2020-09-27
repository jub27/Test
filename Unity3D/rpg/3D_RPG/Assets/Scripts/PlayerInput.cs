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
            if (Physics.Raycast(ray, out hit, 10000f, 1 << LayerMask.NameToLayer("NPC")))
            {
                mousePoint = new Vector3(hit.point.x, transform.position.y, hit.point.z);
            }
            if (Physics.Raycast(ray, out hit, 10000f, 1 << LayerMask.NameToLayer("Enemy")))
            {
                mousePoint = new Vector3(hit.point.x, transform.position.y, hit.point.z);
                cm.SetTarget(hit.collider.gameObject);
            }
        }
        if (Input.GetKeyDown(KeyCode.I)){
            if (InventorySystem.instance.gameObject.activeSelf == false)
                InventorySystem.instance.InventoryOpen();
            else
                InventorySystem.instance.InventoryClose();
        }
        if (Input.GetKeyDown(KeyCode.C))
        {
            if (CharacterInfoSystem.instance.gameObject.activeSelf == false)
                CharacterInfoSystem.instance.CheacterInfoOpen();
            else
                CharacterInfoSystem.instance.CheacterInfoClose();
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
