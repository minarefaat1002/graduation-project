from django.http import HttpResponse
from django.shortcuts import render

# Create your views here.


def auth(request):
    if request.user.is_authenticated:
        return HttpResponse(status=200)
    else:
        return HttpResponse(status=401)


def login(request):
    return HttpResponse("Please login first!!!")
