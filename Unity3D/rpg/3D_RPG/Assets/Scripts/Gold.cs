using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Gold : MonoBehaviour
{
    public GameObject Effect;
    private Rigidbody rigid;
    private TextMeshPro tm;
    private int gold = 0;
    // Start is called before the first frame update
    void Start()
    {
        rigid = GetComponent<Rigidbody>();
        tm = GetComponentInChildren<TextMeshPro>();
        tm.color = new Color(255, 241, 184, 125);
        gold = Random.Range(10, 100);
        tm.text = gold.ToString() + " Gold";
        GameObject temp = Instantiate(Effect, transform.position, transform.rotation);
        temp.transform.parent = transform;
        float x = Random.Range(-4, 4);
        float y = Random.Range(10, 15);
        float z = Random.Range(-4, 4);
        rigid.AddForce(new Vector3(x, y, z), ForceMode.Impulse);
    }

    // Update is called once per frame
    void Update()
    {
        tm.gameObject.transform.forward = Camera.main.transform.forward;
    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            PlayerStatus.instance.UpdateGold(gold);
            Destroy(gameObject);
        }
    }
}
