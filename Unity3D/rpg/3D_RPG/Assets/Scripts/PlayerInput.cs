using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class PlayerInput : MonoBehaviour
{
    CharacterController characterController;
    PlayerControl cm;
    Animator playerAnimator;
    PlayerStatus ps;
    public GameObject escMenu;
    // Start is called before the first frame update
    void Start()
    {
        ps = GetComponent<PlayerStatus>();
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
            PlayerControl.instance.startLoad = true;
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            Vector3 mousePoint = new Vector3();
            RaycastHit hit;
            if (!EventSystem.current.IsPointerOverGameObject())
            {
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
        }

        if (Input.GetKeyDown(KeyCode.Q))
        {
            if (PlayerStatus.instance.curMp > 10)
            {
                if (cm.Skill(0))
                    PlayerStatus.instance.curMp -= 10;
            }
        }
        if (Input.GetKeyDown(KeyCode.W))
        {
            if (PlayerStatus.instance.curMp > 30)
            {
                if(cm.Skill(1))
                    PlayerStatus.instance.curMp -= 30;
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
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            if (InventorySystem.instance.quickSlot1.item_id == 1) // hp포션
            {
                ps.DrinkHpPotion(50);
                InventorySystem.instance.quickSlot1.item_nums--;
                InventorySystem.instance.quickSlot1.itemNums_Text.text = "x" + InventorySystem.instance.quickSlot1.item_nums.ToString();
                if (InventorySystem.instance.quickSlot1.item_nums == 0)
                {
                    InventorySystem.instance.quickSlot1.itemNums_Text.text = "";
                    InventorySystem.instance.quickSlot1.UnSetItem();
                }
            }
            else if (InventorySystem.instance.quickSlot1.item_id == 2)// mp포션
            {
                ps.DrinkMpPotion(50);
                InventorySystem.instance.quickSlot1.item_nums--;
                InventorySystem.instance.quickSlot1.itemNums_Text.text = "x" + InventorySystem.instance.quickSlot1.item_nums.ToString();
                if (InventorySystem.instance.quickSlot1.item_nums == 0)
                {
                    InventorySystem.instance.quickSlot1.itemNums_Text.text = "";
                    InventorySystem.instance.quickSlot1.UnSetItem();
                }
            }
        }

        if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            if (InventorySystem.instance.quickSlot2.item_id == 1) // hp포션
            {
                ps.DrinkHpPotion(50);
                InventorySystem.instance.quickSlot2.item_nums--;
                InventorySystem.instance.quickSlot2.itemNums_Text.text = "x" + InventorySystem.instance.quickSlot2.item_nums.ToString();
                if (InventorySystem.instance.quickSlot2.item_nums == 0)
                {
                    InventorySystem.instance.quickSlot2.itemNums_Text.text = "";
                    InventorySystem.instance.quickSlot2.UnSetItem();
                }
            }
            else if (InventorySystem.instance.quickSlot2.item_id == 2)// mp포션
            {
                ps.DrinkMpPotion(50);
                InventorySystem.instance.quickSlot2.item_nums--;
                InventorySystem.instance.quickSlot2.itemNums_Text.text = "x" + InventorySystem.instance.quickSlot2.item_nums.ToString();
                if (InventorySystem.instance.quickSlot2.item_nums == 0)
                {
                    InventorySystem.instance.quickSlot2.itemNums_Text.text = "";
                    InventorySystem.instance.quickSlot2.UnSetItem();
                }
            }
        }
    }
}
