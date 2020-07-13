using UnityEngine;

// 게임 점수를 증가시키는 아이템
public class Coin : MonoBehaviour, IItem
{
    private string name;
    private int score; // 증가할 점수
    private ItemData data;

    public void SetData(ItemData data)
    {
        name = data.Name;

        int value = Random.Range(data.ItemValue[0], data.ItemValue[1]);
        score = value;
    }

    public void Use(GameObject target)
    {
        // 게임 매니저로 접근해 점수 추가
        GameManager.instance.AddScore(score);
        // 사용되었으므로, 자신을 파괴
        Destroy(gameObject);
    }
}