using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
public class DragWindow : MonoBehaviour, IPointerDownHandler, IPointerUpHandler
{
    public bool isMoving = false;
    public Vector3 baseMousePosition;

    public void OnPointerDown(PointerEventData eventData)
    {
        isMoving = true;
        baseMousePosition = Input.mousePosition;
    }

    public void OnPointerUp(PointerEventData eventData)
    {
        isMoving = false;
    }
}
