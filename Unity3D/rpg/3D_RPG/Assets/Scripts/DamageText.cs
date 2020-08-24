using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class DamageText : MonoBehaviour
{
    public float moveSpeed = 1.0f;
    public float alphaSpeed = 1.0f;
    public float sizeSpeed = 1.0f;
    TextMeshPro text;
    Color alpha;
    public float damage;
    private float sizePlusTime = 0.3f;
    private float sizeTime = 0;
    // Start is called before the first frame update
    void Start()
    {
        text = GetComponent<TextMeshPro>();
        text.text = damage.ToString();
        alpha = text.color;
        Destroy(this.gameObject, 1.0f);
    }

    // Update is called once per frame
    void Update()
    {
        transform.Translate(new Vector3(0, moveSpeed * Time.deltaTime, 0));
        transform.rotation = Camera.main.transform.rotation;
        alpha.a = Mathf.Lerp(alpha.a, 0, Time.deltaTime * alphaSpeed);
        sizeTime += Time.deltaTime;
        if (sizeTime < sizePlusTime) {
            text.fontSize = Mathf.Lerp(text.fontSize, 100, Time.deltaTime * sizeSpeed);
        }
        else
        {
            text.fontSize = Mathf.Lerp(text.fontSize, 0, Time.deltaTime * sizeSpeed);
        }
        text.color = alpha;
    }
}
