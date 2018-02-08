import torch
import config as C



def conv3x3(n_in=C.FILTERS, n_out=C.FILTERS):
    return torch.nn.Conv2d(n_in, n_out, kernel_size=3, padding=1)


def batchnorm2d(n_features=C.FILTERS):
    return torch.nn.BatchNorm2d(
        num_features=n_features, momentum=C.BATCHNORM_MOMENTUM)


class ResidualBlock(torch.nn.Module):
    def __init__(self):
        super(ResidualBlock, self).__init__()
        self.block = torch.nn.Sequential(
            conv3x3(),
            batchnorm2d(),
            torch.nn.ReLU(),
            conv3x3(),
            batchnorm2d())
        self.relu_out = torch.nn.ReLU()

    def forward(self, x):
        residual = x
        out = self.block(x)
        out += residual
        out = self.relu_out(out)
        return out


class PolicyHead(torch.nn.Module):
    def __init__(self):
        super(PolicyHead, self).__init__()
        self.conv = torch.nn.Conv2d(
            in_channels=C.FILTERS, out_channels=2, kernel_size=1)
        self.batchnorm = batchnorm2d(2)
        self.linear = torch.nn.Linear(C.BOARD_SIZE**2 * 2, C.BOARD_SIZE**2)
        self.softmax = torch.nn.Softmax(dim=1)

    def forward(self, x):
        out = self.conv(x)
        out = self.batchnorm(out)
        out = out.view(-1, C.BOARD_SIZE**2 * 2)
        out = self.linear(out)
        out = self.softmax(out)
        return out


class ValueHead(torch.nn.Module):
    def __init__(self):
        super(ValueHead, self).__init__()
        self.conv = torch.nn.Conv2d(
            in_channels=C.FILTERS, out_channels=1, kernel_size=1)
        self.batchnorm = batchnorm2d(1)
        self.linear1 = torch.nn.Linear(C.BOARD_SIZE**2, C.VALUE_HIDDEN_UNITS)
        self.relu = torch.nn.ReLU()
        self.linear2 = torch.nn.Linear(C.VALUE_HIDDEN_UNITS, 1)
        self.tanh = torch.nn.Tanh()

    def forward(self, x):
        out = self.conv(x)
        out = self.batchnorm(out)
        out = out.view(-1, C.BOARD_SIZE**2)
        out = self.linear1(out)
        out = self.relu(out)
        out = self.linear2(out)
        out = self.tanh(out)
        return out


class ResNet(torch.nn.Module):
    def __init__(self):
        super(ResNet, self).__init__()
        self.resnet = torch.nn.Sequential(
            conv3x3(n_in=2),
            batchnorm2d(),
            ResidualBlock(),
            ResidualBlock(),
            ResidualBlock())
        self.policy_head = PolicyHead()
        self.value_head = ValueHead()

    def forward(self, x):
        out = self.resnet(x)
        policy_out = self.policy_head(out)
        value_out = self.value_head(out)
        return policy_out, value_out