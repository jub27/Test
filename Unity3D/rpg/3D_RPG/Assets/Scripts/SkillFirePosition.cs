using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillFirePosition : MonoBehaviour
{
    private GameObject player;
    public static SkillFirePosition instance = null;

    private void Awake()
    {
        if(instance == null)
        {
            DontDestroyOnLoad(gameObject);
            instance = this;
            GameManager.instance.dontDestroyObjectList.Add(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.Find("Player");
    }

    // Update is called once per frame
    void Update()
    {
        transform.position = player.transform.position + new Vector3(0, 2, 0);
    }
}
