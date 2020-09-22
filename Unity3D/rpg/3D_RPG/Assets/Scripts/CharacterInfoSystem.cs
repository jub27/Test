using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterInfoSystem : MonoBehaviour
{
    static public CharacterInfoSystem instance = null;

    public ItemSlot weaponSlot;
    public ItemSlot armorSlot;

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }
    // Start is called before the first frame update
    void Start()
    {
        gameObject.SetActive(false);
        if (GameManager.instance.is_loaded)
        {
            if (GameManager.instance.load_data.weapon != null && GameManager.instance.load_data.weapon.id != 0)
                weaponSlot.SetItem(GameManager.instance.load_data.weapon);
            else if (GameManager.instance.load_data.armor != null && GameManager.instance.load_data.armor.id != 0)
                armorSlot.SetItem(GameManager.instance.load_data.armor);
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void CheacterInfoOpenClose()
    {
        if (gameObject.activeSelf == false)
            gameObject.SetActive(true);
        else
            gameObject.SetActive(false);
    }
}
