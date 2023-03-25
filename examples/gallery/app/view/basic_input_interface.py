# coding:utf-8
from PyQt5.QtCore import Qt, QSize
from PyQt5.QtWidgets import QWidget, QVBoxLayout, QButtonGroup
from PyQt5.QtMultimedia import QSound
from qfluentwidgets import (PushButton, ToolButton, PrimaryPushButton, HyperlinkButton,
                            ComboBox, RadioButton, CheckBox, Slider)

from .gallery_interface import GalleryInterface
from ..common.translator import Translator


class BasicInputInterface(GalleryInterface):
    """ Basic input interface """

    def __init__(self, parent=None):
        translator = Translator()
        super().__init__(
            title=translator.basicInput,
            subtitle='qfluentwidgets.components.widgets',
            parent=parent
        )

        self.addExampleCard(
            self.tr('A simple button with text content'),
            PushButton('Standard push button'),
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/button/demo.py'
        )

        button = ToolButton('app/resource/images/kunkun.png')
        button.setIconSize(QSize(40, 40))
        button.clicked.connect(lambda: QSound.play(
            'app/resource/audio/ZhiYinJi.wav'))
        button.resize(70, 70)
        self.addExampleCard(
            self.tr('A button with graphical content'),
            button,
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/button/demo.py'
        )

        self.addExampleCard(
            self.tr('Accent style applied to button'),
            PrimaryPushButton('Accent style button'),
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/button/demo.py'
        )

        self.addExampleCard(
            self.tr('A hyperlink button that navigates to a URI'),
            HyperlinkButton('http://github.com', 'GitHub home page'),
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/button/demo.py'
        )

        self.addExampleCard(
            self.tr('A 2-state CheckBox'),
            CheckBox('Two-state CheckBox'),
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/check_box/demo.py'
        )

        checkBox = CheckBox('Three-state CheckBox')
        checkBox.setTristate(True)
        self.addExampleCard(
            self.tr('A 3-state CheckBox'),
            checkBox,
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/check_box/demo.py'
        )

        comboBox = ComboBox()
        comboBox.addItems(['shoko 🥰', '西宫硝子 😊', '一级棒卡哇伊的硝子酱 😘'])
        comboBox.setCurrentIndex(0)
        comboBox.setMinimumWidth(210)
        self.addExampleCard(
            self.tr('A ComboBox with items'),
            comboBox,
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/combo_box/demo.py'
        )

        radioWidget = QWidget()
        radioLayout = QVBoxLayout(radioWidget)
        radioLayout.setContentsMargins(2, 0, 0, 0)
        radioLayout.setSpacing(15)
        radioButton1 = RadioButton('Star Platinum', radioWidget)
        radioButton2 = RadioButton('Crazy Diamond', radioWidget)
        radioButton3 = RadioButton('Soft and Wet', radioWidget)
        buttonGroup = QButtonGroup(radioWidget)
        buttonGroup.addButton(radioButton1)
        buttonGroup.addButton(radioButton2)
        buttonGroup.addButton(radioButton3)
        radioLayout.addWidget(radioButton1)
        radioLayout.addWidget(radioButton2)
        radioLayout.addWidget(radioButton3)
        self.addExampleCard(
            self.tr('A group of RadioButton controls in a button group'),
            radioWidget,
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/radio_button/demo.py'
        )

        slider = Slider(Qt.Horizontal)
        slider.setRange(0, 100)
        slider.setFixedWidth(200)
        self.addExampleCard(
            self.tr('A simple horizontal slider'),
            slider,
            'https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/slider/demo.py'
        )
