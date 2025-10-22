{
    "Objects": [
        {
            "components": {
                "Camera": {
                    "far_plane": 1000.0,
                    "fov": 45.0,
                    "is_active": true,
                    "near_plane": 2.0
                },
                "Model": {
                    "meshes": [
                        {
                            "texturas": [
                                {
                                    "path": "default"
                                }
                            ]
                        }
                    ],
                    "model_path": "default"
                },
                "Transform": {
                    "position": [
                        11.437999725341797,
                        0.0,
                        -0.8999999761581421
                    ],
                    "rotation": [
                        4.0,
                        76.0,
                        0.0
                    ],
                    "scale": [
                        1.0,
                        1.0,
                        1.0
                    ]
                }
            },
            "id": 1,
            "name": "cam"
        },
        {
            "components": {
                "Model": {
                    "meshes": [
                        {
                            "texturas": [
                                {
                                    "path": "bark_0004.jpg"
                                }
                            ]
                        },
                        {
                            "texturas": [
                                {
                                    "path": "DB2X2_L01.png"
                                }
                            ]
                        }
                    ],
                    "model_path": "Tree.obj"
                },
                "Script": {
                    "script_enabled": true,
                    "script_name": "move.spl"
                },
                "Transform": {
                    "position": [
                        0.0,
                        -1.5636008977890015,
                        0.0
                    ],
                    "rotation": [
                        0.0,
                        -0.0,
                        0.0
                    ],
                    "scale": [
                        1.0,
                        1.0,
                        1.0
                    ]
                }
            },
            "id": 2,
            "name": "tree maybe"
        },
        {
            "components": {
                "Model": {
                    "meshes": [
                        {
                            "texturas": [
                                {
                                    "path": "wood.jpg"
                                }
                            ]
                        }
                    ],
                    "model_path": "default"
                },
                "Script": {
                    "script_enabled": true,
                    "script_name": "move.spl"
                },
                "Transform": {
                    "position": [
                        -4.722390174865723,
                        0.0,
                        -10.37767219543457
                    ],
                    "rotation": [
                        0.0,
                        -0.0,
                        0.0
                    ],
                    "scale": [
                        1.0,
                        1.0,
                        1.0
                    ]
                }
            },
            "id": 3,
            "name": "box"
        }
    ],
    "id_counting": 6,
    "skybox_name": "cloudy",
    "wireframe_mode": false
}