using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterInfoSystem : MonoBehaviour
{
    static public CharacterInfoSystem instance = null;

    public ItemSlot weaponSlot;
    public ItemSlot armorSlot;
    private bool check = false;
    private Vector3 offset;
    private void Awake()
    {
        if (instance == null)
            instance = this;
        else
            Destroy(gameObject);
    }
    // Start is called before the first frame update
    void Start()
    {
        gameObject.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (GetComponentInChildren<DragWindow>().isMoving)
        {
            if (check == false)
            {
                check = true;
                offset = transform.position - GetComponentInChildren<DragWindow>().baseMousePosition;
            }
            else
                transform.position = Input.mousePosition + offset;
        }
        else
            check = false;
    }

    public void CheacterInfoOpen()
    {
        gameObject.SetActive(true);
    }

    public void CheacterInfoClose()
    {
        gameObject.SetActive(false);
    }
}
